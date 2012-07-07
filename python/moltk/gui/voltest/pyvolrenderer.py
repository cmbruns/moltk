'''
Created on Jun 19, 2012

@author: brunsc
'''

from scenes import sphereImposterShaderProgram
from camera import CameraPosition
from rotation import Rotation
import glrenderer
from PySide import QtCore
from OpenGL.GL import *
from OpenGL.GLU import *

class PyVolRenderer(glrenderer.GlRenderer):
    def __init__(self):
        glrenderer.GlRenderer.__init__(self)
        self.camera_position = CameraPosition()
        self.actors = []
        self.shader = sphereImposterShaderProgram
        self.background_color = [0.8, 0.8, 1.0, 0.0] # sky blue
        
    def init_gl(self):
        # print "init_gl"
        glEnable(GL_DEPTH_TEST)
        bg = self.background_color
        glClearColor(bg[0], bg[1], bg[2], bg[3])
        glShadeModel(GL_SMOOTH)
        glEnable(GL_COLOR_MATERIAL)
        glMaterialfv(GL_FRONT, GL_SPECULAR, [1.0, 1.0, 1.0, 1.0])
        glMaterialfv(GL_FRONT, GL_SHININESS, [100.0])
        glLightfv(GL_LIGHT0, GL_POSITION, [-100.0, 100.0, 100.0, 0.0])
        glLightfv(GL_LIGHT0, GL_DIFFUSE, [1.0, 1.0, 1.0, 1.0])
        glLightfv(GL_LIGHT0, GL_SPECULAR, [1.0, 1.0, 1.0, 1.0])
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, [1.0, 1.0, 1.0, 0.0])
        glEnable(GL_LIGHTING)
        glEnable(GL_LIGHT0)
        glEnable(GL_CULL_FACE)
        for actor in self.actors:
            actor.init_gl()
        self.shader.init_gl()

    def resize_gl(self, w, h):
        self.camera_position.set_window_size_in_pixels(w, h)
        self.update()
        
    def paint_gl(self):
        glDrawBuffer(GL_BACK)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self.shader.zNear = self.camera_position.zNear
        self.shader.zFar = self.camera_position.zFar
        self.shader.zFocus = self.camera_position.distance_to_focus
        self.shader.background_color = self.background_color
        with self.camera_position:
            for actor in self.actors:
                actor.paint_gl()

    @QtCore.Slot(float)
    def increment_zoom(self, ratio):
        self.camera_position.increment_zoom(ratio)
        self.update()

    @QtCore.Slot(Rotation)
    def increment_rotation(self, r):
        self.camera_position.increment_rotation(r)
        self.update()
        
    @QtCore.Slot(int, int, int)
    def translate_pixel(self, x, y, z):
        self.camera_position.translate_pixel(x, y, z)
        self.update()
