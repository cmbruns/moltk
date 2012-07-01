'''
Created on Jun 19, 2012

@author: brunsc
'''

from PySide.QtCore import QObject
from PySide import QtCore
from OpenGL.GL import glViewport

class GlRenderer(QObject):
    def __init__(self):
        QObject.__init__(self, None)
        self.gl_widget = None
        self.b_is_initialized = False
        self.paint_event_needs_flush = False
        
    def set_gl_widget(self, gl_widget):
        self.gl_widget = gl_widget
        gl_widget.paint_requested.connect(self._paint_gl)
        gl_widget.resize_requested.connect(self._resize_gl)
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
        glViewport(0, 0, w, h)
        self.resize_gl(w, h)
        self.gl_widget.doneCurrent()

    def _paint_gl(self):
        # print "_paint_gl"
        if not self.gl_widget:
            return
        if self.paint_event_needs_flush:
            return
        self.paint_event_needs_flush = True
        QtCore.QCoreApplication.processEvents() # flush out pending paint events
        self.gl_widget.makeCurrent()
        if not self.b_is_initialized:
            self.init_gl()
            self.b_is_initialized = True
        self.paint_gl()
        self.gl_widget.swapBuffers()
        self.gl_widget.doneCurrent()
        self.paint_event_needs_flush = False

    def update(self):
        self.update_requested.emit()

    update_requested = QtCore.Signal()

    # Override this
    def paint_gl(self):
        pass
    
    # Override this
    def resize_gl(self, w, h):
        pass
    
    # Override this
    def init_gl(self):
        pass
    