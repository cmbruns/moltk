from PySide.QtCore import QObject
from PySide import QtCore
from OpenGL.GL import *
from OpenGL.GLUT import *
from math import pi

class TeapotActor(QObject):   
    def paint(self):
        glPushAttrib(GL_POLYGON_BIT) # remember current GL_FRONT_FACE indictor
        glFrontFace(GL_CW) # teapot polygon vertex order is opposite to modern convention
        glColor3f(0.2, 0.2, 0.5) # paint it blue
        glutSolidTeapot(1.0) # thank you GLUT tool kit
        glPopAttrib() # restore GL_FRONT_FACE
    