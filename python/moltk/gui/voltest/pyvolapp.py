from scenes import FiveBallScene, TeapotActor
from pyvolrenderer import PyVolRenderer
from pyvolcanvas import PyVolCanvas
from PySide.QtGui import QApplication, QMainWindow
import sys

class PyVolApp(QApplication):
    def __init__(self):
        QApplication.__init__(self, sys.argv)
        mainWin = QMainWindow()
        mainWin.setWindowTitle("PyVol")
        mainWin.resize(640, 480)
        glCanvas = PyVolCanvas(mainWin)
        renderer = PyVolRenderer()
        renderer.actors.append(FiveBallScene())
        # renderer.actors.append(TeapotActor())
        glCanvas.set_gl_renderer(renderer)
        mainWin.setCentralWidget(glCanvas)
        mainWin.show()
        sys.exit(self.exec_())
