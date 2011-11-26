#    MolTK is a Python and C++ toolkit for protein sequence/structure alignment and visualization
#    Copyright (C) 2011  Christopher M. Bruns
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#   
#    Commercial users should ask about our dual licensing model.
#    For questions contact: cmbruns@rotatingpenguin.com

import sys
from PySide.QtGui import QMainWindow, QApplication
from moltk_ui import Ui_MoltkMainWindow
import platform

class MoltkMainWindow(QMainWindow):
    def __init__(self, parent = None):
        QMainWindow.__init__(self, parent)
        self.ui = Ui_MoltkMainWindow()
        self.ui.setupUi(self)
        # work around Mac menubar bug
        # http://bugs.pyside.org/show_bug.cgi?id=907
        if 'Darwin' == platform.system():
            self.ui.menubar.setParent(None)


class MoltkApp(QApplication):
    def __init__(self):
        QApplication.__init__(self, sys.argv)
        self.setApplicationName("MolTK5")
        self.mainWindow = MoltkMainWindow()
        self.mainWindow.show()
        self.exec_()
        sys.exit()
