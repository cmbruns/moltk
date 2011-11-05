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

# moltk module docstring
"""
moltk is the top level package of the MolTK tool kit, which provides tools for protein sequence/structure alignment and visualization.

The submodule "moltk.gui" contains the pure python moltk application and user interface.  
The PySide package (http://www.pyside.org/) must be installed to use the moltk.gui package.

Do not directly use the submodule "moltk._moltk", which represents the binary extension 
implemented in C++.  The top
level module "moltk" contains all of the symbols from "moltk._moltk", so use "import moltk"
of "from moltk import <whatever>" to get symbols from "moltk._moltk".

All MolTK code is Copyright (C) 2011  Christopher M. Bruns

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

Commercial users should ask about our dual licensing model.
For questions contact: cmbruns@rotatingpenguin.com
"""

import _moltk
from _moltk import *

# Put documentation for _moltk members into moltk docs.
__all__ = []
for s in dir():
    if not s.startswith('_'):
        __all__.append(s)

# moltk._moltk docstring
_moltk.__doc__ = """
Do not directly use the submodule "moltk._moltk".  Use module "moltk" instead.
The _moltk module provides tools for protein sequence/structure alignment and visualization.

This submodule "moltk._moltk" represents the binary extension implemented in C++.  The top
level module "moltk" contains all of the symbols from "moltk._moltk", so use "import moltk"
of "from moltk import <whatever>" to get symbols from "moltk._moltk".

All MolTK code is Copyright (C) 2011  Christopher M. Bruns

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

Commercial users should ask about our dual licensing model.
For questions contact: cmbruns@rotatingpenguin.com
"""

# Hide _moltk submodule
_moltk = None

