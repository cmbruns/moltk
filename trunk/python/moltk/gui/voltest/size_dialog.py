
from size_dialog_ui import Ui_windowSizeDialog
from PySide.QtGui import QDialog
from PySide import QtCore

class SizeDialog(QDialog):
    size_changed = QtCore.Signal(int, int)

    def __init__(self, parent=None):
        QDialog.__init__(self, parent)
        self.ui = Ui_windowSizeDialog()
        self.ui.setupUi(self)
        
    @QtCore.Slot(int)
    def on_widthBox_valueChanged(self, w):
        if 0 == w: return
        h = self.ui.heightBox.value()
        if 0 == h: return
        self.size_changed.emit(w, h)

    @QtCore.Slot(int)
    def on_heightBox_valueChanged(self, h):
        if 0 == h: return
        w = self.ui.widthBox.value()
        if 0 == w: return
        self.size_changed.emit(w, h)
