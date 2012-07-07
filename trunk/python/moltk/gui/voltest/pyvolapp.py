from rotation import Vec3
from scenes import FiveBallScene, TeapotActor, SphereImposter, GlutSphereActor
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
        atoms = []
        with open(file_name, 'r') as f:
            print file_name
            for line in f:
                if line.startswith("ATOM"):
                    x = float(line[30:38])
                    y = float(line[38:46])
                    z = float(line[46:54])
                    atoms.append(Vec3([x,y,z]))
        print len(atoms), "atoms found"
        if len(atoms) > 0:
            ren = self.ui.glCanvas.renderer
            # ren.actors = []
            for atom in atoms:
                ren.actors.append(GlutSphereActor(atom, 1.2))
            ren.actors.append(GlutSphereActor([0,0,0], 1.1))
            ren.update()


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
