from rotation import *
from PySide import QtCore
from PySide.QtGui import QApplication
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
        # initialize sceen size to correct value
        # 2.5 should not change dynamically, unless we do head tracking
        self.distance_to_screen_in_screen_heights = 1.8
        dt = QApplication.desktop()
        self.set_screen_size_in_pixels(dt.width(), dt.height())
        # Object/ground/gl-units measures:
        self.distance_to_focus = 25.0 # changes with zoom level

    @property
    def zNear(self):
        return 0.6 * self.distance_to_focus
    
    @property
    def zFar(self):
        return 2.0 * self.distance_to_focus
        
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
        aperture = (2.0 * 180.0 / pi *
                    abs(atan2(self.window_size_in_pixels[1] / 2.0, 
                              self.distance_to_screen_in_pixels)))
        gluPerspective (aperture, # vertical aperture angle in degrees
                        w / h, # aspect ratio
                        self.zNear, # near clip
                        self.zFar) # far clip
        glMatrixMode(GL_MODELVIEW)
        glPushMatrix()
        glLoadIdentity()
        
        f_g = self.focus_in_ground
        c_f = Vec3([0, 0, self.distance_to_focus]) # camera in focus frame
        c_g = f_g + self.rotation * c_f # camera in ground_frame
        # For unrestricted rotation, rotate "up" vector too
        u_f = Vec3([0, 1, 0])
        u_g = self.rotation * u_f
        # Camera distance and focus position
        # Notice that all zooming is accomplished with camera position,
        # i.e. no glScaled(...) here
        gluLookAt(c_g.x, c_g.y, c_g.z, # camera in ground
                  f_g.x, f_g.y, f_g.z, # focus in ground
                  u_g.x, u_g.y, u_g.z) # up vector
        glMatrixMode(GL_MODELVIEW)
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
        self.rotation = self.rotation * ~r

    @QtCore.Slot(float)
    def increment_zoom(self, ratio):
        if ratio <= 0.0:
            return
        self.distance_to_focus /= ratio
        
    @QtCore.Slot(int, int, int)
    def translate_pixel(self, x, y, z):
        # convert to ground coordinates
        glunits_per_pixel = (self.distance_to_focus / 
                             self.distance_to_screen_in_pixels)
        dx = Vec3([x, y, z]) * glunits_per_pixel
        self.focus_in_ground += self.rotation * dx

    @QtCore.Slot(int, int)
    def set_window_size_in_pixels(self, w, h):
        # keep roughly the same view as before by zooming
        old_h = self.window_size_in_pixels[1]
        if old_h is not None:
            self.increment_zoom(h / old_h)
        self.window_size_in_pixels = [1.0*w, 1.0*h]
        
    @QtCore.Slot(int, int)
    def set_screen_size_in_pixels(self, w, h):
        if 0 == h:
            return
        self.screen_size_in_pixels = [1.0*w, 1.0*h]
        self.distance_to_screen_in_pixels = self.distance_to_screen_in_screen_heights * self.screen_size_in_pixels[1]
