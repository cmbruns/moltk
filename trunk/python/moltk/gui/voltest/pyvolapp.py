import stereo3d
from rotation import Vec3
from scenes import FiveBallScene, TeapotActor, SphereImposter, GlutSphereActor
from pyvolrenderer import PyVolRenderer
from pyvolcanvas import PyVolCanvas
from pyvol_ui import Ui_MainWindow
from PySide.QtGui import QApplication, QMainWindow, QFileDialog, QActionGroup
from PySide import QtCore
import platform
import sys


class PyVolMainWindow(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        if platform.system() == "Darwin":
            self.ui.menubar.setParent(None) # Show menu on mac
        stereoActionGroup = QActionGroup(self)
        stereoActionGroup.addAction(self.ui.actionMono_None)
        stereoActionGroup.addAction(self.ui.actionRight_Left_cross_eye)
        stereoActionGroup.addAction(self.ui.actionLeft_Right_parallel)
        stereoActionGroup.addAction(self.ui.actionLeft_eye_view)
        stereoActionGroup.addAction(self.ui.actionRight_eye_view)
        stereoActionGroup.addAction(self.ui.actionRed_Cyan_anaglyph)
        stereoActionGroup.addAction(self.ui.actionGreen_Magenta_anaglyph)
        stereoActionGroup.addAction(self.ui.actionQuadro_120_Hz)
        stereoActionGroup.addAction(self.ui.actionRow_interleaved)
        stereoActionGroup.addAction(self.ui.actionColumn_interleaved)
        stereoActionGroup.addAction(self.ui.actionChecker_interleaved)

    def set_stereo_mode(self, mode, checked):
        if checked:
            self.ui.glCanvas.renderer.stereo_mode = mode
        else:
            # Turn off one stereo mode, turn on Mono
            self.ui.glCanvas.renderer.stereo_mode = stereo3d.Mono()
            self.ui.actionMono_None.setChecked(True)
        self.ui.glCanvas.update()
        
    @QtCore.Slot(bool)
    def on_actionMono_None_triggered(self, checked):
        # If the user clicks on Mono, always use mono, even if it was already checked
        self.set_stereo_mode(stereo3d.Mono(), checked)

    @QtCore.Slot(bool)
    def on_actionRight_Left_cross_eye_triggered(self, checked):
        print "right left", checked
        self.set_stereo_mode(stereo3d.RightLeft(), checked)

    @QtCore.Slot(bool)
    def on_actionLeft_Right_parallel_triggered(self, checked):
        self.set_stereo_mode(stereo3d.LeftRight(), checked)

    @QtCore.Slot(bool)
    def on_actionLeft_eye_view_triggered(self, checked):
        self.set_stereo_mode(stereo3d.Left(), checked)

    @QtCore.Slot(bool)
    def on_actionRight_eye_view_triggered(self, checked):
        self.set_stereo_mode(stereo3d.Right(), checked)

    @QtCore.Slot(bool)
    def on_actionRed_Cyan_anaglyph_triggered(self, checked):
        self.set_stereo_mode(stereo3d.RedCyan(), checked)

    @QtCore.Slot(bool)
    def on_actionGreen_Magenta_anaglyph_triggered(self, checked):
        self.set_stereo_mode(stereo3d.GreenMagenta(), checked)

    @QtCore.Slot(bool)
    def on_actionSwap_Eyes_triggered(self, checked):
        self.ui.glCanvas.renderer.camera_position.swap_eyes = checked
        self.ui.glCanvas.update()

    @QtCore.Slot(bool)
    def on_actionSave_image_triggered(self, checked):
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
