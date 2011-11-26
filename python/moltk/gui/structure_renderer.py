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

class StructureRenderer(Renderer3D):
    """
    StructureRenderer is a custom renderer class that performs all OpenGL
    operations in a separate thread.
    """
    def __init__(self, glwidget, parent = None):
        Renderer3D.__init__(self, glwidget, parent)

