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
# from renderer_3d import Renderer3D
from moltk import Rotation3D, UnitVector3D, radian
from moltk.override import override
from PySide import QtCore
from PySide.QtOpenGL import QGLWidget
from PySide.QtGui import QWidget
from PySide.QtCore import QThread, QCoreApplication
import math

class Canvas3D(QGLWidget):
    """
    Widget for displaying 3D molecule structure in the MolTK GUI.
    
    Don't put OpenGL calls in this class, because all OpenGL calls
    must be called from a dedicated OpenGL thread.
    """
    def __init__(self, parent = None):
        QGLWidget.__init__(self, parent)
        # Maintain rotation state
        self.old_mouse_position = None
        self.rotation = Rotation3D()
        # Delegate OpenGL calls to a separate thread and renderer class
        self.opengl_thread = QThread(self)
        # CLean up OpenGL thread before exiting, to avoid scary message
        QCoreApplication.instance().aboutToQuit.connect(self.clean_up_before_quit)
    
    def set_renderer(self, renderer):
        self.renderer = renderer
        self.renderer.set_gl_widget(self)
        self.renderer.moveToThread(self.opengl_thread)
        self.opengl_thread.start()
    
    # Because we want all OpenGL operations in a separate thread,
    # we need to override paintEvent and resizeEvent to 
    # avoid unwanted GUI thread OpenGL calls
    @override(QWidget)
    def paintEvent(self, event):
        self.doneCurrent()
        self.paint_requested.emit()

    @override(QWidget)
    def resizeEvent(self, event):
        self.doneCurrent()
        sz = event.size()
        self.resize_requested.emit(sz.width(), sz.height())

    @override(QWidget)
    def mouseMoveEvent(self, event):
        if self.old_mouse_position:
            d_pos = event.pos() - self.old_mouse_position
            #
            d_x = d_pos.x()
            d_y = d_pos.y()
            # self.rot_y.emit(d_x * 0.20)
            #
            rot_angle_per_pixel = 2.0 * 3.14159 / (self.width() + self.height() / 2.0)
            rot_angle_per_pixel = rot_angle_per_pixel * radian
            drag_distance = math.sqrt(d_x*d_x + d_y*d_y)
            rot_angle = drag_distance * rot_angle_per_pixel
            rot_axis = UnitVector3D(d_y, d_x, 0)
            drag_rotation = Rotation3D()
            drag_rotation.set_from_angle_about_unit_vector(rot_angle, rot_axis)
            # print "rotation0 = ", self.rotation
            # print "drag_rotation = ", drag_rotation
            self.rotation = drag_rotation * self.rotation
            # print "rotation1 = ", self.rotation
            # print self.rotation
            self.rotation_changed.emit(self)
        # print event.pos()
        self.old_mouse_position = event.pos()

    @override(QWidget)
    def mousePressEvent(self, event):
        self.old_mouse_position = None

    @override(QWidget)
    def mouseReleaseEvent(self, event):
        self.old_mouse_position = None
        
    @QtCore.Slot()
    def clean_up_before_quit(self):
        self.opengl_thread.quit()
        self.opengl_thread.wait()

    paint_requested = QtCore.Signal()
    resize_requested = QtCore.Signal(int, int)
    rotation_changed = QtCore.Signal(QGLWidget)
