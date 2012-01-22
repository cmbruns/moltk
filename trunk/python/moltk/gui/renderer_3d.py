#    MolTK is a Python and C++ toolkit for protein sequence/structure alignment and visualization
#    Copyright (C) 2011  Christopher M. Bruns
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#   
#    Commercial users should ask about our dual licensing model.
#    For questions contact: cmbruns@rotatingpenguin.com

# Do not import anything from PySide.QtGui in this file,
# to help avoid mixing QtGui thread with OpenGL thread.
from __future__ import division
from PySide import QtCore
from PySide.QtCore import QObject
from OpenGL.GL import *
from OpenGL.GLU import *
# Avoid red errors in eclipse by explicitly listing OpenGL imports
from OpenGL.GL import glClearColor, glEnable, glDepthMask, glCullFace, glClear, glMatrixMode, glPushMatrix, \
    glPopMatrix, glLoadIdentity, glTranslatef, glRotatef, glViewport
from OpenGL.GL import GL_DEPTH_TEST, GL_TRUE, GL_CULL_FACE, GL_BACK, GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, \
    GL_PROJECTION, GL_MODELVIEW
from OpenGL.GLU import gluPerspective
import math

class Renderer3D(QObject):
    """
    Renderer3D is a custom renderer class that performs all OpenGL
    operations in a separate thread.
    """
    def __init__(self, gl_widget, parent = None):
        QObject.__init__(self, parent)
        self.gl_widget = gl_widget
        self.gl_widget.paint_called.connect(self.on_paint_called)
        self.gl_widget.resize_called.connect(self.on_resize_called)
        self.x_rot = self.y_rot = self.z_rot = 0.0
        self.init_gl_called = False

    def on_rotation_changed(self, gl_widget):
        # print self
        # print gl_widget.rotation
        if gl_widget:
            angles = gl_widget.rotation.convertToBodyFixedXYZAngles()
            (self.x_rot, self.y_rot, self.z_rot) = angles
            # print (self.x_rot, self.y_rot, self.z_rot)
            self.update()
        
    def update(self):
        self.scene_changed.emit()
        
    def initGL(self):
        print "renderer_3d.initGL"
        self.gl_widget.makeCurrent()
        #
        glClearColor(0.3, 0.2, 0, 1)
        # glEnable(GL_DEPTH_TEST)
        # glDepthMask(GL_TRUE)
        # glEnable(GL_CULL_FACE)
        # glCullFace(GL_BACK)
        self.init_gl_called = True
        #
        self.gl_widget.doneCurrent()

    def paint_background(self):
        print "renderer_3d.paint_background"
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)        
        
    def set_up_camera(self):
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(30, self.aspect, 1, 500)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        glTranslatef(0, 0, -5)
        glRotatef(self.x_rot * 180.0 / 3.14159, 1.0, 0.0, 0.0)
        glRotatef(self.y_rot * 180.0 / 3.14159, 0.0, 1.0, 0.0)
        glRotatef(self.z_rot * 180.0 / 3.14159, 0.0, 0.0, 1.0)
        
    def paint_opaque(self):
        pass

    def paint_transparent(self):
        pass
        
    def paint(self):
        print "renderer_3d.paint"
        self.paint_monoscopic()
        
    def paint_monoscopic(self):
        print "renderer_3d.paint_monoscopic"
        # TODO - eventual paint order should be
        # 1 - paint background color or image or world box
        # 2 - paint opaque geometry
        # 3 - paint transparent geometry in painter order
        # 4 - optionally paint semi-transparent cross hair
        # 5 - paint 2D overlays such as scale bar
        self.aspect = float(self.width) / float(self.height)
        if self.aspect <= 0: return
        if not self.init_gl_called:
            self.initGL()
        self.paint_background()
        glMatrixMode(GL_PROJECTION)
        glPushMatrix()
        glMatrixMode(GL_MODELVIEW)
        glPushMatrix()
        self.set_up_camera()
        self.paint_opaque()
        self.paint_transparent()
        glMatrixMode(GL_PROJECTION)
        glPopMatrix()
        glMatrixMode(GL_MODELVIEW)
        glPopMatrix()
        print "swap"
        self.gl_widget.swapBuffers()

    def on_paint_called(self):
        print "renderer_3d.on_paint_called"
        # assert QThread.currentThread() == glthread
        # assert QThread.currentThread() != guithread
        self.gl_widget.makeCurrent()
        # print "paint"
        self.paint()
        self.gl_widget.doneCurrent()

    def on_resize_called(self, w, h):
        self.width = w
        self.height = h
        self.sqrt_aspect_ratio = math.sqrt(float(w) / float(h))
        # assert QThread.currentThread() == glthread
        # assert QThread.currentThread() != guithread
        self.gl_widget.makeCurrent()
        print "resize", w, h
        glViewport(0, 0, w, h)
        # a paint event will arrive soon...
        self.gl_widget.doneCurrent()

    scene_changed = QtCore.Signal()
