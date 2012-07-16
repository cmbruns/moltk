from main_window import MainWindow
from shader import SphereImposter
from scenes import FiveBallScene, TeapotActor, GlutSphereActor
from pyvolrenderer import PyVolRenderer
from PySide.QtGui import QApplication
from PySide import QtCore, QtGui
import sys

class PyVolApp(QApplication):
    def __init__(self):
        QApplication.__init__(self, sys.argv)
        mainWin = MainWindow()
        renderer = PyVolRenderer()
        renderer.actors.append(FiveBallScene())
        # renderer.actors.append(TeapotActor())
        mainWin.ui.glCanvas.set_gl_renderer(renderer)
        mainWin.show()
        sys.exit(self.exec_())
