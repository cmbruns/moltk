'''
Created on Jun 19, 2012

@author: brunsc
'''

from trackball import Trackball
from PySide.QtOpenGL import QGLWidget
from PySide.QtCore import QThread, QCoreApplication
from PySide import QtCore

class PyVolCanvas(QGLWidget):
    # Singleton thread for all OpenGL actions
    
    def __init__(self, parent=None):
        QGLWidget.__init__(self, parent)
        self.opengl_thread = QThread()
        QCoreApplication.instance().aboutToQuit.connect(self.clean_up_before_quit)
        self.trackball = Trackball()
        
    # delegate opengl tasks to separate non-qt-gui Renderer object
    def set_gl_renderer(self, renderer):
        self.renderer = renderer
        self.renderer.set_gl_widget(self)
        self.renderer.moveToThread(self.opengl_thread)
        self.opengl_thread.start()
        # self.trackball.y_rotated.connect(self.renderer.rotate_y)
        self.trackball.rotation_incremented.connect(self.renderer.increment_rotation)
        self.trackball.zoom_incremented.connect(self.renderer.increment_zoom)
        
    def paintEvent(self, event):
        self.doneCurrent()
        self.paint_requested.emit()
        
    def resizeEvent(self, event):
        self.doneCurrent()
        sz = event.size()
        self.resize_requested.emit(sz.width(), sz.height())
        
    @QtCore.Slot()
    def clean_up_before_quit(self):
        self.opengl_thread.quit()
        self.opengl_thread.wait()

    paint_requested = QtCore.Signal()
    resize_requested = QtCore.Signal(int, int)
    
    # Delegate mouse events to trackball class
    def mouseMoveEvent(self, event):
        self.trackball.mouseMoveEvent(event, self.size())
        
    def mousePressEvent(self, event):
        self.trackball.mousePressEvent(event)
        
    def mouseReleaseEvent(self, event):
        self.trackball.mouseReleaseEvent(event)
        
    def wheelEvent(self, event):
        self.trackball.wheelEvent(event)
