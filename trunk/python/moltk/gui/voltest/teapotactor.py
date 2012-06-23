
from PySide.QtCore import QObject
from PySide import QtCore
from OpenGL.GL import *
from OpenGL.GLUT import *
from math import pi

class TeapotActor(QObject):
    def __init__(self):
        self.rot_y = 0.0
        
    def paint(self):
        glPushAttrib(GL_POLYGON_BIT | GL_TRANSFORM_BIT) # remember current GL_FRONT_FACE indictor
        glMatrixMode(GL_MODELVIEW)
        glPushMatrix()
        glRotated(self.rot_y * 180.0 / pi, 0, 1, 0)
        glFrontFace(GL_CW) # teapot polygon vertex order is opposite to modern convention
        glColor3f(0.2, 0.2, 0.5) # paint it blue
        glutSolidTeapot(2.0) # thank you GLUT tool kit
        glPopMatrix()
        glPopAttrib() # restore GL_FRONT_FACE
        
    @QtCore.Slot(float)
    def rotate_y(self, angle):
        self.rot_y += angle
    