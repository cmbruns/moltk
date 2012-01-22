
from PySide import QtCore
from PySide.QtCore import QObject
from OpenGL.GL import glViewport
import math

class Renderer3D(QObject):
    "Renderer class for calling OpenGL from a dedicated thread"
    def __init__(self, parent=None):
        QObject.__init__(self, parent)
        self.gl_widget = None
        self.b_is_initialized = False
        
    def set_gl_widget(self, gl_widget):
        self.gl_widget = gl_widget
        gl_widget.paint_requested.connect(self._paint_gl)
        gl_widget.resize_requested.connect(self._resize_gl)

    @QtCore.Slot()
    def _resize_gl(self, w, h):
        if not self.gl_widget:
            return
        self.gl_widget.makeCurrent()
        if not self.b_is_initialized:
            self.init_gl()
            self.b_is_initialized = True
        glViewport(0, 0, w, h)
        self.sqrt_aspect_ratio = math.sqrt(float(w) / float(h))
        self.gl_widget.doneCurrent()

    @QtCore.Slot()
    def _paint_gl(self):
        if not self.gl_widget:
            return
        self.gl_widget.makeCurrent()
        if not self.b_is_initialized:
            self.init_gl()
            self.b_is_initialized = True
        self.paint_stereo()
        self.gl_widget.swapBuffers()
        self.gl_widget.doneCurrent()

    def paint_stereo(self):
        # TODO
        self.paint()

    def paint(self):
        pass # Override this
    
    def init_gl(self):
        pass # Override this
    
    scene_changed = QtCore.Signal()
    
