#!/usr/bin/python
'''
Created on Jan 16, 2012

@author: cmbruns
'''

from override import override
from PySide.QtGui import QMainWindow, QApplication
from PySide.QtOpenGL import QGLWidget
import math
import sys

from OpenGL.GL import *

class Redbook1_2(QGLWidget):
    def __init__(self, parent=None):
        QGLWidget.__init__(self, parent)
        self.sqrt_aspect_ratio = 1.0
        
    @override(QGLWidget)
    def initializeGL(self):
        glClearColor(0.5, 0, 0, 0)       
    
    @override(QGLWidget)
    def resizeGL(self, w, h):
        self.sqrt_aspect_ratio = math.sqrt(float(w) / float(h))
        glViewport(0, 0, w, h)

    @override(QGLWidget)
    def paintGL(self):
        glClear(GL_COLOR_BUFFER_BIT)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glOrtho(0.0, self.sqrt_aspect_ratio, 0.0, 1.0/self.sqrt_aspect_ratio, -1.0, 1.0)
        self.paint_white_square()
        
    def paint_white_square(self):
        glColor3f(1,1,1)
        if glBegin(GL_POLYGON) or True:
            glVertex3f(0.25, 0.25, 0.0)
            glVertex3f(0.75, 0.25, 0.0)
            glVertex3f(0.75, 0.75, 0.0)
            glVertex3f(0.25, 0.75, 0.0)
            glEnd()
        
class TestGLApp(QApplication):
    def __init__(self):
        QApplication.__init__(self, sys.argv)
        self.gl_widget = Redbook1_2()
        self.mainWindow = QMainWindow()
        self.mainWindow.setCentralWidget(self.gl_widget)
        self.mainWindow.resize(250, 250)
        self.mainWindow.show()
        self.exec_()
        sys.exit()
        
if __name__ == '__main__':
    TestGLApp()
