#!/usr/bin/python

from moltk.gui.gl.renderer_3d import Renderer3D
from moltk.override import override
from OpenGL.GL import *
from OpenGL.GL import glVertex3f, glMatrixMode, glPopMatrix, glPushMatrix, glLoadIdentity, \
        glOrtho, glClearColor, glClear, glColor3f, glBegin, glEnd
from OpenGL.GL import GL_PROJECTION, GL_COLOR_BUFFER_BIT, GL_POLYGON

class Example2_1Renderer(Renderer3D):
    @override(Renderer3D)
    def paint(self):
        self.paint_white_square()
        
    @override(Renderer3D)
    def init_gl(self):
        glClearColor(0,0,0.2,0)

    def paint_white_square(self):
        glMatrixMode(GL_PROJECTION)
        glPushMatrix()
        glLoadIdentity()
        # Use square root o faspect ratio to compromise beween proportional height and proportioaal width
        # Apply a little translation to keep center at 0.5
        sx = self.sqrt_aspect_ratio
        sy = 1.0 / self.sqrt_aspect_ratio
        tx = (1.0 - sx) / 2.0
        ty = (1.0 - sy) / 2.0
        glOrtho(tx, tx + sx, # X range
                ty, ty + sy, # Y range
                -1.0, 1.0) # Z range
        glClear(GL_COLOR_BUFFER_BIT)
        glColor3f (1.0, 0.8, 0.8)
        glBegin(GL_POLYGON)
        glVertex3f (0.25, 0.25, 0.0)
        glVertex3f (0.75, 0.25, 0.0)
        glVertex3f (0.75, 0.75, 0.0)
        glVertex3f (0.25, 0.75, 0.0)
        glEnd()
        glPopMatrix()

