'''
Created on Jun 19, 2012

@author: brunsc
'''

import stereo3d
from PySide.QtCore import QObject
from PySide import QtCore
from OpenGL.GL import glViewport
from math import pi, atan2
import os

class GlRenderer(QObject):
    "GlRenderer make OpenGL calls in a separate thread on behalf of a QGLWidget"
    
    update_requested = QtCore.Signal()

    def __init__(self):
        QObject.__init__(self, None)
        self.gl_widget = None
        self.b_is_initialized = False
        self.paint_event_needs_flush = False
        
    def set_gl_widget(self, gl_widget):
        self.gl_widget = gl_widget
        gl_widget.paint_requested.connect(self._paint_gl)
        gl_widget.resize_requested.connect(self._resize_gl)
        gl_widget.save_image_requested.connect(self.save_image)
        gl_widget.save_lenticular_series_requested.connect(self.save_lenticular_series)
        self.update_requested.connect(gl_widget.update)
        
    @QtCore.Slot()
    def _resize_gl(self, w, h):
        # print "_resize_gl", w, h
        if not self.gl_widget:
            return
        self.gl_widget.makeCurrent()
        if not self.b_is_initialized:
            self.init_gl()
            self.b_is_initialized = True
        self.resize_gl(w, h)
        self.gl_widget.doneCurrent()

    def _paint_gl(self):
        # print "_paint_gl"
        if not self.gl_widget:
            return
        if self.paint_event_needs_flush:
            return
        self.paint_event_needs_flush = True
        # Only processEvents() if we use a separate OpenGL thread
        # QtCore.QCoreApplication.processEvents() # flush out pending paint events
        self.gl_widget.makeCurrent()
        if not self.b_is_initialized:
            self.init_gl()
            self.b_is_initialized = True
        self.paint_gl()
        self.gl_widget.swapBuffers()
        self.gl_widget.doneCurrent()
        self.paint_event_needs_flush = False

    @QtCore.Slot(str)
    def save_image(self, file_name):
        self.gl_widget.makeCurrent()
        image = self.gl_widget.grabFrameBuffer()
        image.save(file_name)
        print "saved", file_name
        self.gl_widget.doneCurrent()

    @QtCore.Slot(str, float, int)
    def save_lenticular_series(self, file_name, angle, count):
        a = angle/2.0
        da = -angle / (count - 1.0)
        radians_per_eye = atan2(0.5 * self.camera_position.iod, self.camera_position.zFocus)
        froot, fext = os.path.splitext(file_name)
        self.stereo_mode = stereo3d.Minimal() # so I can modify eye_pos
        for i in range(count):
            n = '_%03d' % (i + 1)
            fn = froot + n + fext
            eye_pos = a / radians_per_eye
            self.camera_position.eye_pos = eye_pos
            self.gl_widget.makeCurrent()
            self.paint_gl()
            print i, a, eye_pos, fn
            self.save_image(fn)
            a += da

    def update(self):
        self.update_requested.emit()

    # Override this
    def paint_gl(self):
        pass
    
    # Override this
    def resize_gl(self, w, h):
        pass
    
    # Override this
    def init_gl(self):
        pass
