
class Mono:
    def views(self, camera):
        camera.viewport_xrelstart = 0.0
        camera.viewport_relwidth = 1.0
        camera.eye_pos = 0.0 # middle eye
        with camera:
            yield camera


class Left(Mono):
    def views(self, camera):
        # Left eye view on left side of viewport
        camera.viewport_xrelstart = 0.0
        camera.viewport_relwidth = 1.0
        camera.eye_pos = -1.0 # left eye
        with camera:
            yield camera


class Right(Mono):
    def views(self, camera):
        # Left eye view on left side of viewport
        camera.viewport_xrelstart = 0.0
        camera.viewport_relwidth = 1.0
        camera.eye_pos = -1.0 # left eye
        with camera:
            yield camera


class LeftRight(Mono):
    def views(self, camera):
        # Left eye view on left side of viewport
        camera.viewport_xrelstart = 0.0
        camera.viewport_relwidth = 0.5
        camera.eye_pos = -1.0 # left eye
        with camera:
            yield camera
        # Right eye view on right side of viewport
        camera.viewport_xrelstart = 0.5
        camera.viewport_relwidth = 0.5
        camera.eye_pos = +1.0 # right eye
        with camera:
            yield camera


class RightLeft(Mono):
    def views(self, camera):
        # Left eye view on left side of viewport
        camera.viewport_xrelstart = 0.5
        camera.viewport_relwidth = 0.5
        camera.eye_pos = -1.0 # left eye
        with camera:
            yield camera
        # Right eye view on right side of viewport
        camera.viewport_xrelstart = 0.0
        camera.viewport_relwidth = 0.5
        camera.eye_pos = +1.0 # right eye
        with camera:
            yield camera

