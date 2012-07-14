# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'size_dialog.ui'
#
# Created: Sat Jul 14 19:37:58 2012
#      by: pyside-uic 0.2.14 running on PySide 1.1.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_windowSizeDialog(object):
    def setupUi(self, windowSizeDialog):
        windowSizeDialog.setObjectName("windowSizeDialog")
        windowSizeDialog.resize(267, 113)
        self.verticalLayout = QtGui.QVBoxLayout(windowSizeDialog)
        self.verticalLayout.setObjectName("verticalLayout")
        self.comboBox = QtGui.QComboBox(windowSizeDialog)
        self.comboBox.setObjectName("comboBox")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.comboBox.addItem("")
        self.verticalLayout.addWidget(self.comboBox)
        self.frame = QtGui.QFrame(windowSizeDialog)
        self.frame.setFrameShape(QtGui.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtGui.QFrame.Raised)
        self.frame.setObjectName("frame")
        self.horizontalLayout = QtGui.QHBoxLayout(self.frame)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.label_2 = QtGui.QLabel(self.frame)
        self.label_2.setObjectName("label_2")
        self.horizontalLayout.addWidget(self.label_2)
        self.widthBox = QtGui.QSpinBox(self.frame)
        self.widthBox.setMinimumSize(QtCore.QSize(70, 0))
        self.widthBox.setMaximum(20000)
        self.widthBox.setProperty("value", 640)
        self.widthBox.setObjectName("widthBox")
        self.horizontalLayout.addWidget(self.widthBox)
        self.label = QtGui.QLabel(self.frame)
        self.label.setObjectName("label")
        self.horizontalLayout.addWidget(self.label)
        self.heightBox = QtGui.QSpinBox(self.frame)
        self.heightBox.setMinimumSize(QtCore.QSize(70, 0))
        self.heightBox.setMaximum(20000)
        self.heightBox.setProperty("value", 480)
        self.heightBox.setObjectName("heightBox")
        self.horizontalLayout.addWidget(self.heightBox)
        self.label_3 = QtGui.QLabel(self.frame)
        self.label_3.setObjectName("label_3")
        self.horizontalLayout.addWidget(self.label_3)
        self.verticalLayout.addWidget(self.frame)
        self.buttonBox = QtGui.QDialogButtonBox(windowSizeDialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.verticalLayout.addWidget(self.buttonBox)

        self.retranslateUi(windowSizeDialog)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("accepted()"), windowSizeDialog.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("rejected()"), windowSizeDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(windowSizeDialog)

    def retranslateUi(self, windowSizeDialog):
        windowSizeDialog.setWindowTitle(QtGui.QApplication.translate("windowSizeDialog", "Enter size of display window", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(0, QtGui.QApplication.translate("windowSizeDialog", "Choose resolution", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(1, QtGui.QApplication.translate("windowSizeDialog", "1080p: 1920x1080 (HDTV)", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(2, QtGui.QApplication.translate("windowSizeDialog", "720p: 1280x720 (HDTV)", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(3, QtGui.QApplication.translate("windowSizeDialog", "480p: 854x480 (youtube)", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(4, QtGui.QApplication.translate("windowSizeDialog", "720x480 (DVD NTSC)", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(5, QtGui.QApplication.translate("windowSizeDialog", "360p: 640x360", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(6, QtGui.QApplication.translate("windowSizeDialog", "240p: 426x240", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("windowSizeDialog", "Width", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("windowSizeDialog", "x", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("windowSizeDialog", "Height", None, QtGui.QApplication.UnicodeUTF8))

