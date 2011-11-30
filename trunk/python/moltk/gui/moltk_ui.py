# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'moltk.ui'
#
# Created: Thu Nov 10 10:20:30 2011
#      by: pyside-uic 0.2.9 running on PySide 1.0.3
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_MoltkMainWindow(object):
    def setupUi(self, MoltkMainWindow):
        MoltkMainWindow.setObjectName("MoltkMainWindow")
        MoltkMainWindow.resize(498, 493)
        self.centralwidget = QtGui.QWidget(MoltkMainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayout = QtGui.QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.frame = QtGui.QFrame(self.centralwidget)
        self.frame.setFrameShape(QtGui.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtGui.QFrame.Raised)
        self.frame.setObjectName("frame")
        self.horizontalLayout_2 = QtGui.QHBoxLayout(self.frame)
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.splitter = QtGui.QSplitter(self.frame)
        self.splitter.setFrameShape(QtGui.QFrame.StyledPanel)
        self.splitter.setFrameShadow(QtGui.QFrame.Raised)
        self.splitter.setOrientation(QtCore.Qt.Vertical)
        self.splitter.setOpaqueResize(True)
        self.splitter.setHandleWidth(5)
        self.splitter.setObjectName("splitter")
        self.frame_2 = QtGui.QFrame(self.splitter)
        self.frame_2.setFrameShape(QtGui.QFrame.StyledPanel)
        self.frame_2.setFrameShadow(QtGui.QFrame.Raised)
        self.frame_2.setLineWidth(5)
        self.frame_2.setObjectName("frame_2")
        self.widget_3 = StructureCanvas(self.splitter)
        self.widget_3.setObjectName("widget_3")
        self.horizontalLayout_2.addWidget(self.splitter)
        self.horizontalLayout.addWidget(self.frame)
        MoltkMainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MoltkMainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 498, 22))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")
        self.menuHelp = QtGui.QMenu(self.menubar)
        self.menuHelp.setObjectName("menuHelp")
        self.menuEdit = QtGui.QMenu(self.menubar)
        self.menuEdit.setObjectName("menuEdit")
        self.menuView = QtGui.QMenu(self.menubar)
        self.menuView.setObjectName("menuView")
        MoltkMainWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MoltkMainWindow)
        self.statusbar.setObjectName("statusbar")
        MoltkMainWindow.setStatusBar(self.statusbar)
        self.actionOpen = QtGui.QAction(MoltkMainWindow)
        self.actionOpen.setObjectName("actionOpen")
        self.actionBrowse_MolTK_python_API = QtGui.QAction(MoltkMainWindow)
        self.actionBrowse_MolTK_python_API.setObjectName("actionBrowse_MolTK_python_API")
        self.menuFile.addAction(self.actionOpen)
        self.menuHelp.addAction(self.actionBrowse_MolTK_python_API)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuEdit.menuAction())
        self.menubar.addAction(self.menuView.menuAction())
        self.menubar.addAction(self.menuHelp.menuAction())

        self.retranslateUi(MoltkMainWindow)
        QtCore.QMetaObject.connectSlotsByName(MoltkMainWindow)

    def retranslateUi(self, MoltkMainWindow):
        MoltkMainWindow.setWindowTitle(QtGui.QApplication.translate("MoltkMainWindow", "MolTK: (no molecules)", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MoltkMainWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuHelp.setTitle(QtGui.QApplication.translate("MoltkMainWindow", "Help", None, QtGui.QApplication.UnicodeUTF8))
        self.menuEdit.setTitle(QtGui.QApplication.translate("MoltkMainWindow", "Edit", None, QtGui.QApplication.UnicodeUTF8))
        self.menuView.setTitle(QtGui.QApplication.translate("MoltkMainWindow", "View", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setText(QtGui.QApplication.translate("MoltkMainWindow", "Open...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionBrowse_MolTK_python_API.setText(QtGui.QApplication.translate("MoltkMainWindow", "Browse MolTK python API", None, QtGui.QApplication.UnicodeUTF8))

from structure_canvas import StructureCanvas
