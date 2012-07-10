from rotation import *
from PySide import QtCore
from PySide.QtGui import QApplication
from OpenGL.GL import *
from OpenGL.GLU import *
from math import pi, atan2, floor

class CameraPosition(QtCore.QObject):
    """
    Values can appear in various reference frames.  Each frame has 
      * an origin
      * a unit of measure or a scale
      * an orientation of its three axes 
    * Ground frame: an absolute reference frame for the objects being viewed.
        origin is at some fixed point, possibly the initial focus position.  This
          origin does not vary during user interaction. 
        units are object units, such as nanometers
    * Object frame: internal reference frame of a particular item in the scene.
    * Camera frame: one of the two user-centric frames
        camera/eye is at origin
        units are object units; e.g. angstroms or nanometers for molecules
        negative Z-axis points from the camera toward the focus position
        positive Y-axis points up
    * Eye frame: one of two user-centric frames
        camera/eye is at origin
        units are human-scale units, pixels or meters
        negative Z-axis points from the eye toward the center of the application window
        positive Y-axis points up
    * Clip frame (used in shaders)
        camera frame transformed by projection matrix
        non-linear transformation
        z-values vary between 0.0 at front clip plane and 1.0 at back clip plane
        no longer orthogonal
    """
    def __init__(self):
        # GL units are ground units
        self.rotation = Rotation() # about focus point
        self.focus_in_ground = Vec3([0,0,0])
        # Human/real-space/user measures:
        self.window_size_in_pixels = [640.0, 480.0] # will be overwritten on first resize event
        # initialize sceen size to correct value
        # ratio of screen distance to screen height:
        # measured 2.7 for my home monitor
        # 2.5 for Cami's monitor
        # 5.0 for our TV
        # should not change dynamically, unless we do head tracking
        self.distance_to_screen_in_screen_heights = 2.5
        dt = QApplication.desktop()
        self.set_screen_size_in_pixels(dt.width(), dt.height())
        # Object/ground/gl-units measures:
        self.distance_to_focus = 25.0 # changes with zoom level
        # viewport parameters for side by side stereo
        self.viewport_xrelstart = 0.0
        self.viewport_yrelstart = 0.0
        self.viewport_relwidth = 1.0
        self.viewport_relheight = 1.0
        # big value for testing
        self.interocular_distance_in_meters = 0.062 # Probably close enough for anyone
        self.pixels_per_meter = 3724; # TODO - expose this
        self.interocular_distance_in_pixels = self.pixels_per_meter * self.interocular_distance_in_meters;
        self.eye_pos = 0.0 # -1.0 for left eye, +1.0 for right eye
        self.swap_eyes = False

    @property
    def zNear(self):
        "distance to front clipping plane in camera frame"
        return 0.6 * self.distance_to_focus
    
    @property
    def zFar(self):
        "distance to rear clipping plane in camera frame"
        return 2.5 * self.distance_to_focus
    
    @property
    def zFocus(self):
        "distance to center of trackball rotation in camera frame"
        return self.distance_to_focus
        
    @property
    def glunits_per_pixel(self):
        return self.distance_to_focus / self.distance_to_screen_in_pixels

    @property
    def iod(self):
        "interocular distance in ground"
        return self.interocular_distance_in_pixels * self.glunits_per_pixel
        
    @property
    def viewport_xstart(self):
        return int(self.viewport_xrelstart * self.window_size_in_pixels[0])
    
    @property
    def viewport_ystart(self):
        return int(self.viewport_yrelstart * self.window_size_in_pixels[1])
    
    @property
    def viewport_width(self):
        return self.window_size_in_pixels[0] * self.viewport_relwidth

    @property
    def viewport_height(self):
        return self.window_size_in_pixels[1] * self.viewport_relheight

    @property
    def aspect_ratio(self):
        "ratio of viewport width to height"
        return self.viewport_width / self.viewport_height

    @property
    def yFov(self):
        "vertical aperture angle from camera to viewport screen in radians"
        return 2.0 * abs(atan2(self.viewport_height / 2.0, 
                         self.distance_to_screen_in_pixels))

    @property
    def yFov_degrees(self):
        return self.yFov * 180.0 / pi

    @property
    def rotation(self):
        return self._R_gf
    
    @rotation.setter
    def rotation(self, r):
        self._R_gf = r
        self.rotAngle, self.rotAxis = r.to_angle_axis()
        
    @property
    def eye_shift_in_ground(self):
        shift = -0.5 * self.eye_pos * self.iod
        if self.swap_eyes:
            shift *= -1
        return shift
        
    def set_gl_projection(self):
        # http://www.orthostereo.com/geometryopengl.html
        glMatrixMode(GL_PROJECTION)
        bUseAsymmetricFrustum = True
        if bUseAsymmetricFrustum:
            topFrustum = 0.5 * self.zNear * self.viewport_height / self.distance_to_screen_in_pixels
            bottomFrustum = -topFrustum
            right = self.aspect_ratio * topFrustum
            frustumShift = self.eye_shift_in_ground * self.zNear / self.zFocus
            rightFrustum = right + frustumShift
            leftFrustum = -right + frustumShift
            glFrustum(leftFrustum, rightFrustum,
                      bottomFrustum, topFrustum,
                      self.zNear, self.zFar)
            glTranslatef(self.eye_shift_in_ground, 0, 0) # translate to cancel parallax
        else:
            gluPerspective (self.yFov_degrees, # vertical aperture angle in degrees
                            self.aspect_ratio, # aspect ratio
                            self.zNear, # near clip
                            self.zFar) # far clip
        
    def set_gl_viewport(self):
        # Indirection so we can create side-by-side stereo in sub viewports
        glViewport(self.viewport_xstart, self.viewport_ystart, 
                   int(self.viewport_width), int(self.viewport_height)) # fill window        

    def __enter__(self):
        self.set_gl_viewport()
        # Remember previous settings for later
        glPushAttrib(GL_TRANSFORM_BIT)
        glMatrixMode(GL_PROJECTION)
        glPushMatrix()
        glLoadIdentity() # reset projection matrix
        self.set_gl_projection()
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
        dx = Vec3([x, y, z]) * self.glunits_per_pixel
        self.focus_in_ground += self.rotation * dx
        
    @QtCore.Slot(int, int, int)
    def center_pixel(self, x, y, z):
        # If there are multiple viewports, which is this in?
        relx = float(x) / self.window_size_in_pixels[0] / self.viewport_relwidth
        vx = int(floor(relx)) # horizontal viewport index
        x2 = int((relx - vx) * self.viewport_width)
        cx = 0.5 * self.viewport_width
        dx = x2 - cx
        rely = float(y) / self.window_size_in_pixels[1] / self.viewport_relheight
        vy = int(floor(rely))
        y2 = int((rely - vy) * self.viewport_height)
        cy = 0.5 * self.viewport_height
        dy = cy - y2 # flip y
        self.translate_pixel(dx, dy, 0)
        

    @QtCore.Slot(int, int)
    def set_window_size_in_pixels(self, w, h):
        # keep roughly the same view as before by zooming
        old_h = self.viewport_height
        self.window_size_in_pixels = [1.0*w, 1.0*h]
        if old_h is not None:
            self.increment_zoom(self.viewport_height / old_h)
        
    @QtCore.Slot(int, int)
    def set_screen_size_in_pixels(self, w, h):
        if 0 == h:
            return
        self.screen_size_in_pixels = [1.0*w, 1.0*h]
        self.distance_to_screen_in_pixels = self.distance_to_screen_in_screen_heights * self.screen_size_in_pixels[1]
