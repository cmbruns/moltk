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
from moltk import Rotation3D
from PySide import QtCore
from PySide.QtCore import QObject
from OpenGL.GL import *
from OpenGL.GLU import *

class StructureRenderer(QObject):
    """
    StructureRenderer is a custom renderer class that performs all OpenGL
    operations in a separate thread.
    """
    def __init__(self, glwidget, parent = None):
        QObject.__init__(self, parent)
        self.glwidget = glwidget
        self.glwidget.paintCalled.connect(self.onPaintCalled)
        self.glwidget.resizeCalled.connect(self.onResizeCalled)
        self.xRot = self.yRot = self.zRot = 0.0
        self.initGLCalled = False

    def onRotationChanged(self, glwidget):
        # print self
        # print glwidget.rotation
        if glwidget:
            angles = glwidget.rotation.convertToBodyFixedXYZAngles()
            (self.xRot, self.yRot, self.zRot) = angles
            # print (self.xRot, self.yRot, self.zRot)
            self.update()
        
    def update(self):
        self.sceneChanged.emit()
        
    def initGL(self):
        self.glwidget.makeCurrent()
        #
        glClearColor(0.3, 0.2, 0, 1)
        glEnable(GL_DEPTH_TEST)
        glDepthMask(GL_TRUE)
        glEnable(GL_CULL_FACE)
        glCullFace(GL_BACK)
        self.initGLCalled = True
        #
        self.glwidget.doneCurrent()

    def paintBackground(self):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)        
        
    def setupCamera(self):
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(30, self.aspect, 1, 500)
        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()
        glTranslatef(0, 0, -5)
        glRotatef(self.xRot * 180.0 / 3.14159, 1.0, 0.0, 0.0)
        glRotatef(self.yRot * 180.0 / 3.14159, 0.0, 1.0, 0.0)
        glRotatef(self.zRot * 180.0 / 3.14159, 0.0, 0.0, 1.0)
        
    def paintOpaqueGeometry(self):
        pass

    def paintTransparentGeometry(self):
        pass
        
    def paint(self):
        self.paint_monoscopic()
        
    def paint_monoscopic(self):
        # TODO - eventual paint order should be
        # 1 - paint background color or image or world box
        # 2 - paint opaque geometry
        # 3 - paint transparent geometry in painter order
        # 4 - optionally paint semi-transparent cross hair
        # 5 - paint 2D overlays such as scale bar
        self.aspect = float(self.width) / float(self.height)
        if self.aspect <= 0: return
        if not self.initGLCalled:
            self.initGL()
        self.paintBackground()
        glMatrixMode(GL_PROJECTION)
        glPushMatrix()
        glMatrixMode(GL_MODELVIEW)
        glPushMatrix()
        self.setupCamera()
        self.paintOpaqueGeometry()
        self.paintTransparentGeometry()
        glMatrixMode(GL_PROJECTION)
        glPopMatrix()
        glMatrixMode(GL_MODELVIEW)
        glPopMatrix()
        self.glwidget.swapBuffers()

    def onPaintCalled(self):
        # assert QThread.currentThread() == glthread
        # assert QThread.currentThread() != guithread
        self.glwidget.makeCurrent()
        # print "paint"
        self.paint()
        self.glwidget.doneCurrent()

    def onResizeCalled(self, w, h):
        self.width = w
        self.height = h
        # assert QThread.currentThread() == glthread
        # assert QThread.currentThread() != guithread
        self.glwidget.makeCurrent()
        # print "resize", w, h
        glViewport(0, 0, w, h)
        # a paint event will arrive soon...
        self.glwidget.doneCurrent()

    sceneChanged = QtCore.Signal()
