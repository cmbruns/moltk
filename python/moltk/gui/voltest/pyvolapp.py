from scenes import FiveBallScene, TeapotActor
from pyvolrenderer import PyVolRenderer
from pyvolcanvas import PyVolCanvas
from pyvol_ui import Ui_MainWindow
from PySide.QtGui import QApplication, QMainWindow, QFileDialog
from PySide import QtCore
import sys


class PyVolMainWindow(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

    @QtCore.Slot()        
    def on_actionOpen_triggered(self):
        file_name, type = QFileDialog.getOpenFileName(
                self, 
                "Open PDB file", 
                None, 
                self.tr("PDB Files(*.pdb)"))
        if file_name == "":
            return
        with open(file_name, 'r') as f:
            print file_name
            pass


class PyVolApp(QApplication):
    def __init__(self):
        QApplication.__init__(self, sys.argv)
        mainWin = PyVolMainWindow()
        renderer = PyVolRenderer()
        renderer.actors.append(FiveBallScene())
        # renderer.actors.append(TeapotActor())
        mainWin.ui.glCanvas.set_gl_renderer(renderer)
        mainWin.show()
        sys.exit(self.exec_())
