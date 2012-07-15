# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'pyvol.ui'
#
# Created: Sun Jul 15 02:47:18 2012
#      by: pyside-uic 0.2.14 running on PySide 1.1.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtGui.QWidget(MainWindow)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Preferred, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.centralwidget.sizePolicy().hasHeightForWidth())
        self.centralwidget.setSizePolicy(sizePolicy)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayout = QtGui.QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.glCanvas = PyVolCanvas(self.centralwidget)
        self.glCanvas.setObjectName("glCanvas")
        self.horizontalLayout.addWidget(self.glCanvas)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 21))
        self.menubar.setObjectName("menubar")
        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")
        self.menuView = QtGui.QMenu(self.menubar)
        self.menuView.setObjectName("menuView")
        self.menuStereo_3D = QtGui.QMenu(self.menuView)
        self.menuStereo_3D.setObjectName("menuStereo_3D")
        self.menuBookmarks = QtGui.QMenu(self.menuView)
        self.menuBookmarks.setObjectName("menuBookmarks")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.actionOpen = QtGui.QAction(MainWindow)
        self.actionOpen.setObjectName("actionOpen")
        self.actionSave_image = QtGui.QAction(MainWindow)
        self.actionSave_image.setObjectName("actionSave_image")
        self.actionSet_size = QtGui.QAction(MainWindow)
        self.actionSet_size.setEnabled(True)
        self.actionSet_size.setObjectName("actionSet_size")
        self.actionAdd_bookmark = QtGui.QAction(MainWindow)
        self.actionAdd_bookmark.setEnabled(False)
        self.actionAdd_bookmark.setObjectName("actionAdd_bookmark")
        self.actionMono_None = QtGui.QAction(MainWindow)
        self.actionMono_None.setCheckable(True)
        self.actionMono_None.setChecked(True)
        self.actionMono_None.setObjectName("actionMono_None")
        self.actionRight_Left_cross_eye = QtGui.QAction(MainWindow)
        self.actionRight_Left_cross_eye.setCheckable(True)
        self.actionRight_Left_cross_eye.setEnabled(True)
        self.actionRight_Left_cross_eye.setObjectName("actionRight_Left_cross_eye")
        self.actionLeft_Right_parallel = QtGui.QAction(MainWindow)
        self.actionLeft_Right_parallel.setCheckable(True)
        self.actionLeft_Right_parallel.setEnabled(True)
        self.actionLeft_Right_parallel.setObjectName("actionLeft_Right_parallel")
        self.actionLeft_eye_view = QtGui.QAction(MainWindow)
        self.actionLeft_eye_view.setCheckable(True)
        self.actionLeft_eye_view.setObjectName("actionLeft_eye_view")
        self.actionRight_eye_view = QtGui.QAction(MainWindow)
        self.actionRight_eye_view.setCheckable(True)
        self.actionRight_eye_view.setObjectName("actionRight_eye_view")
        self.actionRow_interleaved = QtGui.QAction(MainWindow)
        self.actionRow_interleaved.setEnabled(False)
        self.actionRow_interleaved.setObjectName("actionRow_interleaved")
        self.actionColumn_interleaved = QtGui.QAction(MainWindow)
        self.actionColumn_interleaved.setEnabled(False)
        self.actionColumn_interleaved.setObjectName("actionColumn_interleaved")
        self.actionChecker_interleaved = QtGui.QAction(MainWindow)
        self.actionChecker_interleaved.setEnabled(False)
        self.actionChecker_interleaved.setObjectName("actionChecker_interleaved")
        self.actionSwap_Eyes = QtGui.QAction(MainWindow)
        self.actionSwap_Eyes.setCheckable(True)
        self.actionSwap_Eyes.setEnabled(True)
        self.actionSwap_Eyes.setObjectName("actionSwap_Eyes")
        self.actionRed_Cyan_anaglyph = QtGui.QAction(MainWindow)
        self.actionRed_Cyan_anaglyph.setCheckable(True)
        self.actionRed_Cyan_anaglyph.setEnabled(True)
        self.actionRed_Cyan_anaglyph.setObjectName("actionRed_Cyan_anaglyph")
        self.actionGreen_Magenta_anaglyph = QtGui.QAction(MainWindow)
        self.actionGreen_Magenta_anaglyph.setCheckable(True)
        self.actionGreen_Magenta_anaglyph.setEnabled(True)
        self.actionGreen_Magenta_anaglyph.setObjectName("actionGreen_Magenta_anaglyph")
        self.actionQuadro_120_Hz = QtGui.QAction(MainWindow)
        self.actionQuadro_120_Hz.setEnabled(False)
        self.actionQuadro_120_Hz.setObjectName("actionQuadro_120_Hz")
        self.actionSave_Lenticular_Series = QtGui.QAction(MainWindow)
        self.actionSave_Lenticular_Series.setObjectName("actionSave_Lenticular_Series")
        self.actionAdd_new_bookmark = QtGui.QAction(MainWindow)
        self.actionAdd_new_bookmark.setObjectName("actionAdd_new_bookmark")
        self.actionGo_to_previous_bookmark = QtGui.QAction(MainWindow)
        self.actionGo_to_previous_bookmark.setObjectName("actionGo_to_previous_bookmark")
        self.actionGo_to_next_bookmark = QtGui.QAction(MainWindow)
        self.actionGo_to_next_bookmark.setObjectName("actionGo_to_next_bookmark")
        self.actionClear_all_bookmarks = QtGui.QAction(MainWindow)
        self.actionClear_all_bookmarks.setObjectName("actionClear_all_bookmarks")
        self.actionPlay_movie = QtGui.QAction(MainWindow)
        self.actionPlay_movie.setObjectName("actionPlay_movie")
        self.actionSave_movie = QtGui.QAction(MainWindow)
        self.actionSave_movie.setObjectName("actionSave_movie")
        self.actionQuit = QtGui.QAction(MainWindow)
        self.actionQuit.setMenuRole(QtGui.QAction.QuitRole)
        self.actionQuit.setObjectName("actionQuit")
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionSave_image)
        self.menuFile.addAction(self.actionSave_Lenticular_Series)
        self.menuFile.addAction(self.actionSave_movie)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionQuit)
        self.menuStereo_3D.addAction(self.actionMono_None)
        self.menuStereo_3D.addSeparator()
        self.menuStereo_3D.addAction(self.actionRed_Cyan_anaglyph)
        self.menuStereo_3D.addAction(self.actionGreen_Magenta_anaglyph)
        self.menuStereo_3D.addSeparator()
        self.menuStereo_3D.addAction(self.actionRight_Left_cross_eye)
        self.menuStereo_3D.addAction(self.actionLeft_Right_parallel)
        self.menuStereo_3D.addAction(self.actionLeft_eye_view)
        self.menuStereo_3D.addAction(self.actionRight_eye_view)
        self.menuStereo_3D.addSeparator()
        self.menuStereo_3D.addAction(self.actionQuadro_120_Hz)
        self.menuStereo_3D.addAction(self.actionRow_interleaved)
        self.menuStereo_3D.addAction(self.actionColumn_interleaved)
        self.menuStereo_3D.addAction(self.actionChecker_interleaved)
        self.menuStereo_3D.addSeparator()
        self.menuStereo_3D.addAction(self.actionSwap_Eyes)
        self.menuBookmarks.addAction(self.actionPlay_movie)
        self.menuBookmarks.addSeparator()
        self.menuBookmarks.addAction(self.actionAdd_new_bookmark)
        self.menuBookmarks.addAction(self.actionGo_to_previous_bookmark)
        self.menuBookmarks.addAction(self.actionGo_to_next_bookmark)
        self.menuBookmarks.addAction(self.actionClear_all_bookmarks)
        self.menuView.addAction(self.menuBookmarks.menuAction())
        self.menuView.addAction(self.menuStereo_3D.menuAction())
        self.menuView.addAction(self.actionSet_size)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuView.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "PyVol", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MainWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuView.setTitle(QtGui.QApplication.translate("MainWindow", "View", None, QtGui.QApplication.UnicodeUTF8))
        self.menuStereo_3D.setTitle(QtGui.QApplication.translate("MainWindow", "Stereo 3D", None, QtGui.QApplication.UnicodeUTF8))
        self.menuBookmarks.setTitle(QtGui.QApplication.translate("MainWindow", "Bookmarks", None, QtGui.QApplication.UnicodeUTF8))
        self.actionOpen.setText(QtGui.QApplication.translate("MainWindow", "Load molecule...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_image.setText(QtGui.QApplication.translate("MainWindow", "Save image...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSet_size.setText(QtGui.QApplication.translate("MainWindow", "Set window size...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAdd_bookmark.setText(QtGui.QApplication.translate("MainWindow", "Bookmark this view", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMono_None.setText(QtGui.QApplication.translate("MainWindow", "Mono/None", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRight_Left_cross_eye.setText(QtGui.QApplication.translate("MainWindow", "Right:Left cross-eye", None, QtGui.QApplication.UnicodeUTF8))
        self.actionLeft_Right_parallel.setText(QtGui.QApplication.translate("MainWindow", "Left:Right parallel", None, QtGui.QApplication.UnicodeUTF8))
        self.actionLeft_eye_view.setText(QtGui.QApplication.translate("MainWindow", "Left eye view", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRight_eye_view.setText(QtGui.QApplication.translate("MainWindow", "Right eye view", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRow_interleaved.setText(QtGui.QApplication.translate("MainWindow", "Row interleaved", None, QtGui.QApplication.UnicodeUTF8))
        self.actionColumn_interleaved.setText(QtGui.QApplication.translate("MainWindow", "Column interleaved", None, QtGui.QApplication.UnicodeUTF8))
        self.actionChecker_interleaved.setText(QtGui.QApplication.translate("MainWindow", "Checker interleaved", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSwap_Eyes.setText(QtGui.QApplication.translate("MainWindow", "Swap Eyes", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRed_Cyan_anaglyph.setText(QtGui.QApplication.translate("MainWindow", "Red:Cyan anaglyph", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGreen_Magenta_anaglyph.setText(QtGui.QApplication.translate("MainWindow", "Green:Magenta anaglyph", None, QtGui.QApplication.UnicodeUTF8))
        self.actionQuadro_120_Hz.setText(QtGui.QApplication.translate("MainWindow", "Quadro 120 Hz", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_Lenticular_Series.setText(QtGui.QApplication.translate("MainWindow", "Save Lenticular Series...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAdd_new_bookmark.setText(QtGui.QApplication.translate("MainWindow", "Add new bookmark", None, QtGui.QApplication.UnicodeUTF8))
        self.actionAdd_new_bookmark.setShortcut(QtGui.QApplication.translate("MainWindow", "Ctrl+D", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGo_to_previous_bookmark.setText(QtGui.QApplication.translate("MainWindow", "Go to previous bookmark", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGo_to_previous_bookmark.setShortcut(QtGui.QApplication.translate("MainWindow", "PgUp", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGo_to_next_bookmark.setText(QtGui.QApplication.translate("MainWindow", "Go to next bookmark", None, QtGui.QApplication.UnicodeUTF8))
        self.actionGo_to_next_bookmark.setShortcut(QtGui.QApplication.translate("MainWindow", "PgDown", None, QtGui.QApplication.UnicodeUTF8))
        self.actionClear_all_bookmarks.setText(QtGui.QApplication.translate("MainWindow", "Clear all bookmarks", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPlay_movie.setText(QtGui.QApplication.translate("MainWindow", "Play movie", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSave_movie.setText(QtGui.QApplication.translate("MainWindow", "Save movie...", None, QtGui.QApplication.UnicodeUTF8))
        self.actionQuit.setText(QtGui.QApplication.translate("MainWindow", "Quit", None, QtGui.QApplication.UnicodeUTF8))

from pyvolcanvas import PyVolCanvas
