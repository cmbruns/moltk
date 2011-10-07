# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'moltk.ui'
#
# Created: Thu Oct 06 18:02:45 2011
#      by: pyside-uic 0.2.10 running on PySide 1.0.4
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_MoltkMainWindow(object):
    def setupUi(self, MoltkMainWindow):
        MoltkMainWindow.setObjectName("MoltkMainWindow")
        MoltkMainWindow.resize(498, 493)
        self.centralwidget = QtGui.QWidget(MoltkMainWindow)
        self.centralwidget.setObjectName("centralwidget")
        MoltkMainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MoltkMainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 498, 21))
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
        self.menuFile.addAction(self.actionOpen)
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

