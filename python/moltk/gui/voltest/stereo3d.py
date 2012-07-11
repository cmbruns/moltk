
from OpenGL.GL import glColorMask, glPushAttrib, glPopAttrib, GL_COLOR_BUFFER_BIT


class Minimal:
    def views(self, camera):
        with camera:
            yield camera


class Mono(Minimal):
    def set_defaults(self, camera):
        camera.viewport_xrelstart = 0.0
        camera.viewport_relwidth = 1.0
        camera.eye_pos = 0.0 # middle eye        
        glColorMask(True, True, True, True)
        
    def views(self, camera):
        self.set_defaults(camera)
        with camera:
            yield camera


class Left(Mono):
    def views(self, camera):
        self.set_defaults(camera)
        camera.eye_pos = -1.0 # left eye
        with camera:
            yield camera


class Right(Mono):
    def views(self, camera):
        self.set_defaults(camera)
        camera.eye_pos = 1.0 # right eye
        with camera:
            yield camera


class LeftRight(Mono):
    def views(self, camera):
        # Left eye view on left side of viewport
        self.set_defaults(camera)
        camera.viewport_xrelstart = 0.0
        camera.viewport_relwidth = 0.5
        camera.eye_pos = -1.0 # left eye
        with camera:
            yield camera
        # Right eye view on right side of viewport
        camera.viewport_xrelstart = 0.5
        camera.eye_pos = +1.0 # right eye
        with camera:
            yield camera


class RightLeft(Mono):
    def views(self, camera):
        # Left eye view on left side of viewport
        self.set_defaults(camera)
        camera.viewport_xrelstart = 0.5
        camera.viewport_relwidth = 0.5
        camera.eye_pos = -1.0 # left eye
        with camera:
            yield camera
        # Right eye view on right side of viewport
        camera.viewport_xrelstart = 0.0
        camera.eye_pos = +1.0 # right eye
        with camera:
            yield camera
            

class RedCyan(Mono):
    def views(self, camera):
        # Left eye view red
        self.set_defaults(camera)
        camera.eye_pos = -1.0 # left eye
        glColorMask(True, False, False, True) # Red
        with camera:
            yield camera
        # Right eye view cyan
        camera.eye_pos = +1.0 # right eye
        glColorMask(False, True, True, True)
        with camera:
            yield camera


class GreenMagenta(Mono):
    def views(self, camera):
        # Left eye view red
        self.set_defaults(camera)
        camera.eye_pos = -1.0 # left eye
        glColorMask(False, True, False, True) # Red
        with camera:
            yield camera
        # Right eye view cyan
        camera.eye_pos = +1.0 # right eye
        glColorMask(True, False, True, True)
        with camera:
            yield camera
