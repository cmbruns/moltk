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

# Do not import anything from PySide.QtGui in this file,
# to help avoid mixing QtGui thread with OpenGL thread.
from __future__ import division
from renderer_3d import Renderer3D

from OpenGL.GL import *
from OpenGL.GL import glClearColor, glEnable, glDepthMask, glCullFace, glClear, glMatrixMode, glPushMatrix, \
    glPopMatrix, glLoadIdentity, glTranslatef, glRotatef, glViewport, glBegin, glEnd, glVertex3f, glColor3f, \
    glLineWidth, glShadeModel, glLoadIdentity, glScalef, glFlush, glFrustum, glOrtho
from OpenGL.GL import GL_DEPTH_TEST, GL_TRUE, GL_CULL_FACE, GL_BACK, GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, \
    GL_PROJECTION, GL_MODELVIEW, GL_LINES, GL_FLAT, GL_POLYGON
from OpenGL.GLU import gluLookAt
from OpenGL.GLUT import glutWireCube

def override(interface_class):
    """
    To get Java-like derived class method override annotation.
    Courtesy of mkorpela's answer at 
    http://stackoverflow.com/questions/1167617/in-python-how-do-i-indicate-im-overriding-a-method
    """
    def override(method):
        assert(method.__name__ in dir(interface_class))
        return method
    return override


class StructureRenderer(Renderer3D):
    """
    StructureRenderer draws molecular structures using OpenGL
    """
    def __init__(self, gl_widget, parent = None):
        Renderer3D.__init__(self, gl_widget, parent)

    @override(Renderer3D)        
    def initGL(self):
        self.gl_widget.makeCurrent()
        #
        glClearColor(0,0,0,0)
        glShadeModel(GL_FLAT)
        
        # glEnable(GL_DEPTH_TEST)
        # glDepthMask(GL_TRUE)
        # glEnable(GL_CULL_FACE)
        # glCullFace(GL_BACK)
        # glShadeModel(GL_FLAT)
        self.initGLCalled = True
        #
        self.gl_widget.doneCurrent()

    @override(Renderer3D)
    def set_up_camera(self):
        print "set up camera"
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        glMatrixMode(GL_MODELVIEW)
        
    @override(Renderer3D)        
    def paint_background(self):
        print "paint_background"
        glClear(GL_COLOR_BUFFER_BIT)        
        
    @override(Renderer3D)        
    def paint_opaque(self):
        print "paint_opaque"
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
        glClearColor(0,0,0,0)
        glClear(GL_COLOR_BUFFER_BIT)        
        # print "paint_opaque"
        print "Draw square"
        # glColor3f(1,1,0)
        glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0)
        glBegin(GL_POLYGON)
        glVertex3f(0.25, 0.25, 0.0)
        glVertex3f(0.75, 0.25, 0.0)
        glVertex3f(0.75, 0.75, 0.0)
        glVertex3f(0.25, 0.75, 0.0)
        glEnd()
        # glMatrixMode(GL_MODELVIEW)
        # glColor3f(1, 0, 0)
        # glLoadIdentity()
        # gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0)
        # glScalef (1.0, 2.0, 1.0)
        # glutWireCube (1.0)
        # if glBegin(GL_LINES) or True:
        #     glVertex3f(-5, 0, 5)
        #     glVertex3f(5, 0, 5)
        #     glEnd()
        glFlush()

