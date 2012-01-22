#!/usr/bin/python

from moltk.gui.canvas_3d import Canvas3D
from moltk.gui.gl.example_2_1_renderer import Example2_1Renderer
from PySide.QtGui import QApplication, QMainWindow
import sys

class TestApp(QApplication):
    def __init__(self, renderer):
        QApplication.__init__(self, sys.argv)
        self.win = QMainWindow()
        gl_widget = Canvas3D()
        gl_widget.set_renderer(renderer)
        self.win.setCentralWidget(gl_widget)
        self.win.resize(250, 250)
        self.win.show()

if __name__ == "__main__":
    app = TestApp(Example2_1Renderer())
    sys.exit(app.exec_())
