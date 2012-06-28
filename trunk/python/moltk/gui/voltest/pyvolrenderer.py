'''
Created on Jun 19, 2012

@author: brunsc
'''

from teapotactor import TeapotActor
import glrenderer
from PySide import QtCore
from OpenGL.GL import *
from OpenGL.GLU import *

class PyVolRenderer(glrenderer.GlRenderer):
    def init_gl(self):
        # print "init_gl"
        glEnable(GL_DEPTH_TEST)
        glClearColor(0.8, 0.8, 1.0, 0) # sky blue
        glShadeModel(GL_SMOOTH)
        glEnable(GL_COLOR_MATERIAL)
        glMaterialfv(GL_FRONT, GL_SPECULAR, [1.0, 1.0, 1.0, 1.0])
        glMaterialfv(GL_FRONT, GL_SHININESS, [50.0])
        glLightfv(GL_LIGHT0, GL_POSITION, [1.0, 1.0, 1.0, 0.0])
        glLightfv(GL_LIGHT0, GL_DIFFUSE, [1.0, 1.0, 1.0, 1.0])
        glLightfv(GL_LIGHT0, GL_SPECULAR, [1.0, 1.0, 1.0, 1.0])
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, [1.0, 1.0, 1.0, 0.0])
        glEnable(GL_LIGHTING)
        glEnable(GL_LIGHT0)
        self.orient_camera(self.gl_widget.width(), self.gl_widget.height())
        gluLookAt(0, 0, -10, # camera
                  0, 0, 0, # focus
                  0, 1, 0) # up vector
        self.teapot = TeapotActor()

    def orient_camera(self, w, h):
        "update projection matrix, especially when aspect ratio changes"
        glPushAttrib(GL_TRANSFORM_BIT) # remember current GL_MATRIX_MODE
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective (40.0, # aperture angle in degrees
                        w/float(h), # aspect ratio
                        1.0, # near clip
                        20.0) # far clip
        glPopAttrib() # restore GL_MATRIX_MODE

    def resize_gl(self, w, h):
        # print "resize", w, h
        self.orient_camera(w, h)
        
    def paint_gl(self):
        glDrawBuffer(GL_BACK)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self.teapot.paint()

    @QtCore.Slot(float)
    def zoom(self, ratio):
        self.teapot.zoom(ratio)
        self.update_requested.emit()

    @QtCore.Slot(float)
    def rotate_y(self, angle):
        self.teapot.rotate_y(angle)
        self.update_requested.emit()
        
    @QtCore.Slot(float, float, float)
    def rotate_xyz(self, rotx, roty, rotz):
        self.teapot.rotate_xyz(rotx, roty, rotz)
        self.update_requested.emit()
