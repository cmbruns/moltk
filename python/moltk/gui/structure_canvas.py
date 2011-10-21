
# Do not import PyOpenGL in this file,
# to help avoid mixing QtGui thread with OpenGL thread.
from __future__ import division
from structure_renderer import StructureRenderer
from moltk import Displacement, Rotation3D
from PySide import QtCore
from PySide.QtOpenGL import QGLWidget
import math

class StructureCanvas(QGLWidget):
    """
    Widget for displaying 3D molecule structure in the MolTK GUI.
    
    Don't put OpenGL calls in this class, because all OpenGL calls
    must be called from a dedicated OpenGL thread.
    """
    def __init__(self, parent = None):
        QGLWidget.__init__(self, parent)
        self.renderer = StructureRenderer(self, self)
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
            dragDistance = math.sqrt(dx*dx + dy*dy)
            rotAngle = dragDistance * rotAnglePerPixel
            rotAxis = Vector3D([dy, dx, 0]).normalize()
            dragRotation = Rotation3D.fromAngleAboutUnitVector(rotAngle, rotAxis)
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
