from pyvol_ui import Ui_MainWindow
from size_dialog import SizeDialog
from scenes import SphereImposter, GlutSphereActor
from movie import Movie, KeyFrame
import stereo3d
from rotation import Vec3
from PySide import QtCore
from PySide.QtGui import QMainWindow, QFileDialog, QActionGroup
import platform
from math import pi

class MainWindow(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        if platform.system() == "Darwin":
            self.ui.menubar.setParent(None) # Show menu on mac
        self.bookmarks = Movie()
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

    @property
    def camera(self):
        return self.ui.glCanvas.renderer.camera_position

    @QtCore.Slot(bool)
    def on_actionPlay_movie_triggered(self, checked):
        for frame in self.bookmarks.play():
            self.camera.state = frame.camera_state
            self.ui.glCanvas.update()

    @QtCore.Slot(bool)
    def on_actionAdd_new_bookmark_triggered(self, checked):
        self.bookmarks.append(KeyFrame(self.camera.state))
        self.statusBar().showMessage("Added bookmark number " + str(self.bookmarks.current_key_frame_index + 1), 2000)

    @QtCore.Slot(bool)
    def on_actionGo_to_previous_bookmark_triggered(self, checked):
        if len(self.bookmarks) < 1:
            return
        self.bookmarks.decrement()
        self.camera.state = self.bookmarks.current_key_frame.camera_state
        self.ui.glCanvas.update()
        
    @QtCore.Slot(bool)
    def on_actionGo_to_next_bookmark_triggered(self, checked):
        if len(self.bookmarks) < 1:
            return
        self.bookmarks.increment()
        self.camera.state = self.bookmarks.current_key_frame.camera_state
        self.ui.glCanvas.update()
        
    @QtCore.Slot(bool)
    def on_actionClear_all_bookmarks_triggered(self, checked):
        self.bookmarks.clear()
        
    @QtCore.Slot(bool)
    def on_actionSet_size_triggered(self, checked):
        old_size = self.ui.glCanvas.size()
        dialog = SizeDialog(self)
        dialog.ui.widthBox.setValue(old_size.width())
        dialog.ui.heightBox.setValue(old_size.height())
        dialog.size_changed.connect(self.ui.glCanvas.resize)
        dialog.exec_()
        if dialog.result() == QtGui.QDialog.Accepted:
            print "Accepted"
        else:
            self.ui.glCanvas.resize(old_size)

    @QtCore.Slot(bool)
    def on_actionSave_Lenticular_Series_triggered(self, checked):
        file_name, type = QFileDialog.getSaveFileName(
                self, 
                "Save lenticular series", 
                None, 
                # PNG format silently does not work
                self.tr("images(*.tif)"))
        if file_name == "":
            return
        self.ui.glCanvas.save_lenticular_series(file_name=file_name, 
                                                angle=15.0*pi/180.0, 
                                                count=18)
        
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
        self.camera.swap_eyes = checked
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
        image = self.ui.glCanvas.save_image(file_name)

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
            