from rotation import *
from PySide import QtCore
from OpenGL.GL import *
from OpenGL.GLU import *
from math import pi, atan2

class CameraPosition(QtCore.QObject):
    def __init__(self):
        # GL units are ground units
        self.rotation = Rotation() # about focus point
        self.focus_in_ground = Vec3([0,0,0])
        # Human/real-space/user measures:
        self.window_size_in_pixels = [640.0, 480.0] # will be overwritten on first resize event
        # TODO - initialize sceen size to correct value
        self.screen_size_in_pixels = [1920.0, 1080.0]
        # distance measured for my home desktop monitor
        # 2.5 should not change dynamically, unless we do head tracking
        self.distance_to_screen_in_pixels = 2.5 * self.screen_size_in_pixels[1]
        # Object/ground/gl-units measures:
        self.distance_to_focus_in_ground = 10.0 # changes with zoom level
        
    @property
    def rotation(self):
        return self._R_gf
    
    @rotation.setter
    def rotation(self, r):
        self._R_gf = r
        self.rotAngle, self.rotAxis = r.to_angle_axis()
        
    def __enter__(self):
        # Remember previous settings for later
        glPushAttrib(GL_TRANSFORM_BIT)
        # Use perspective projection
        glLoadIdentity()
        glMatrixMode(GL_PROJECTION)
        glPushMatrix()
        w = self.window_size_in_pixels[0]
        h = self.window_size_in_pixels[1]
        glViewport(0, 0, int(w), int(h)) # fill window
        d = self.distance_to_focus_in_ground
        aperture = (2.0 * 180.0 / pi *
                    abs(atan2(self.window_size_in_pixels[1] / 2.0, 
                              self.distance_to_screen_in_pixels)))
        gluPerspective (aperture, # vertical aperture angle in degrees
                        w / h, # aspect ratio
                        d / 5.0, # near clip
                        5.0 * d) # far clip
        glMatrixMode(GL_MODELVIEW)
        glPushMatrix()
        glLoadIdentity()
        
        # Camera distance and focus position
        # Notice that all zooming is accomplished with camera position,
        # i.e. no glScaled(...) here
        gluLookAt(0, 0, -d, # camera
                  0, 0, 0, # focus
                  0, 1, 0) # up vector
        
        f = self.focus_in_ground
        glTranslated(-f.x, -f.y, -f.z)
        
        # Rotation about focus
        if 0.0 != self.rotAngle:
            glRotated(self.rotAngle * 180.0 / pi,
                      self.rotAxis[0],
                      self.rotAxis[1],
                      self.rotAxis[2])
            
        # f = ~self.rotation * self.focus_in_ground
        # glTranslated(-f.x, -f.y, -f.z) 
        
        return self
    
    def __exit__(self, type, value, tb):
        # Recall stored OpenGL state from before __enter__ call
        glMatrixMode(GL_MODELVIEW)
        glPopMatrix()
        glMatrixMode(GL_PROJECTION)
        glPopMatrix()
        glPopAttrib()
        return False
    
    @QtCore.Slot(Rotation)
    def increment_rotation(self, r):
        self.rotation = r * self.rotation

    @QtCore.Slot(float)
    def increment_zoom(self, ratio):
        if ratio <= 0.0:
            return
        self.distance_to_focus_in_ground /= ratio
        
    @QtCore.Slot(int, int, int)
    def translate_pixel(self, x, y, z):
        # convert to ground coordinates
        glunits_per_pixel = (self.distance_to_focus_in_ground / 
                             self.distance_to_screen_in_pixels)
        dx = Vec3([x, y, z]) * glunits_per_pixel
        self.focus_in_ground += dx

    @QtCore.Slot(int, int)
    def set_window_size_in_pixels(self, w, h):
        # keep roughly the same view as before by zooming
        self.increment_zoom(h / self.window_size_in_pixels[1])
        self.window_size_in_pixels = [1.0*w, 1.0*h]
