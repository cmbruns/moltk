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
    def on_actionSave_image_triggered(self):
        print "save image"
        file_name, type = QFileDialog.getSaveFileName(
                self, 
                "Save screen shot", 
                None, 
                # PNG format silently does not work
                self.tr("images(*.jpg *.tif)"))
        if file_name == "":
            return
        image = self.ui.glCanvas.grabFrameBuffer()
        print file_name
        image.save(file_name)

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
                    atom = Vec3([x, y, z])
                    atom.name = line[12:16]
                    atom.color = [0.5, 0, 0.5]
                    atom.radius = 1.0
                    if (atom.name[1:2] == 'H'):
                        atom.color = [0.6, 0.6, 0.6]
                        atom.radius = 1.2
                    if (atom.name[1:2] == 'C'):
                        atom.color = [0.4, 0.4, 0.4]
                        atom.radius = 1.70
                    if (atom.name[1:2] == 'N'):
                        atom.color = [0.2, 0.2, 0.6]
                        atom.radius = 1.55
                    if (atom.name[1:2] == 'O'):
                        atom.color = [0.5, 0.1, 0.1]
                        atom.radius = 1.52
                    atoms.append(atom)
        print len(atoms), "atoms found"
        if len(atoms) > 0:
            ren = self.ui.glCanvas.renderer
            ren.actors = []
            atom_count = 0
            for atom in atoms:
                atom_count += 1
                if 1 == atom_count:
                    # keep track of molecule bounds
                    min = Vec3(atom[:])
                    max = Vec3(atom[:])
                else:
                    for i in range(3):
                        if atom[i] > max[i]:
                            max[i] = atom[i]
                        if atom[i] < min[i]:
                            min[i] = atom[i]
                ren.actors.append(SphereImposter(atom, radius=atom.radius, color=atom.color))
            # center on molecule
            new_focus = 0.5 * (min + max)
            ren.camera_position.focus_in_ground = new_focus
            ren.camera_position.distance_to_focus = 4.0 * (max - min).norm()            
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
