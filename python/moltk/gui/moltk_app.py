import sys
from PySide.QtCore import *
from PySide.QtGui import *
from moltk_ui import Ui_MoltkMainWindow

class MoltkMainWindow(QMainWindow):
    def __init__(self, parent = None):
        QMainWindow.__init__(self, parent)
        self.ui = Ui_MoltkMainWindow()
        self.ui.setupUi(self)


class MoltkApp(QApplication):
    def __init__(self):
        QApplication.__init__(self, sys.argv)
        self.mainWindow = MoltkMainWindow()
        self.mainWindow.show()
        self.exec_()
        sys.exit()
