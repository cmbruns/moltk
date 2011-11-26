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

# Do not import PyOpenGL in this file,
# to help avoid mixing QtGui thread with OpenGL thread.
from __future__ import division
from renderer_3d import Renderer3D
from moltk import Rotation3D, UnitVector3D, radian
from PySide import QtCore
from PySide.QtOpenGL import QGLWidget
import math

class Canvas3D(QGLWidget):
    """
    Widget for displaying 3D molecule structure in the MolTK GUI.
    
    Don't put OpenGL calls in this class, because all OpenGL calls
    must be called from a dedicated OpenGL thread.
    """
    def __init__(self, parent = None):
        QGLWidget.__init__(self, parent)
        self.renderer = Renderer3D(self, self)
        self.oldMousePos = None
        self.rotation = Rotation3D()

    # Because we want all OpenGL operations in a separate thread,
    # we need to override paintEvent and resizeEvent to 
    # avoid unwanted GUI thread OpenGL calls
    def paintEvent(self, event):
        self.doneCurrent()
        self.paintCalled.emit()

    def resizeEvent(self, event):
        self.doneCurrent()
        self.resizeCalled.emit(event.size().width(), event.size().height())

    def mouseMoveEvent(self, event):
        if self.oldMousePos:
            dPos = event.pos() - self.oldMousePos
            #
            dx = dPos.x()
            dy = dPos.y()
            # self.rotY.emit(dx * 0.20)
            #
            rotAnglePerPixel = 2.0 * 3.14159 / (self.width() + self.height() / 2.0)
            rotAnglePerPixel = rotAnglePerPixel * radian
            dragDistance = math.sqrt(dx*dx + dy*dy)
            rotAngle = dragDistance * rotAnglePerPixel
            rotAxis = UnitVector3D(dy, dx, 0)
            dragRotation = Rotation3D()
            dragRotation.set_from_angle_about_unit_vector(rotAngle, rotAxis)
            # print "rotation0 = ", self.rotation
            # print "dragRotation = ", dragRotation
            self.rotation = dragRotation * self.rotation
            # print "rotation1 = ", self.rotation
            # print self.rotation
            self.rotationChanged.emit(self)
        # print event.pos()
        self.oldMousePos = event.pos()

    def mousePressEvent(self, event):
        self.oldMousePos = None

    def mouseReleaseEvent(self, event):
        self.oldMousePos = None

    paintCalled = QtCore.Signal()
    resizeCalled = QtCore.Signal(int, int)
    rotY = QtCore.Signal(float)
    rotationChanged = QtCore.Signal(QGLWidget)
