from rotation import *
from PySide import QtCore
from OpenGL.GL import *
from math import pi

class CameraPosition(QtCore.QObject):
    def __init__(self):
        self.rotation = Rotation()
        
    @property
    def rotation(self):
        return self._R_gf
    
    @rotation.setter
    def rotation(self, r):
        self._R_gf = r
        self.rotAngle, self.rotAxis = r.to_angle_axis()
        
    def __enter__(self):
        glPushAttrib(GL_TRANSFORM_BIT)
        glMatrixMode(GL_MODELVIEW)
        glPushMatrix()
        if 0.0 != self.rotAngle:
            glRotated(self.rotAngle * 180.0 / pi,
                      self.rotAxis[0],
                      self.rotAxis[1],
                      self.rotAxis[2])
        # TODO
        return self
    
    def __exit__(self, type, value, tb):
        glPopMatrix()
        glPopAttrib()
        return False
    
    @QtCore.Slot(Rotation)
    def increment_rotation(self, r):
        self.rotation = r * self.rotation
        
