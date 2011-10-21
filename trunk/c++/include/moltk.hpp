/*
    MolTK is a Python and C++ toolkit for protein sequence/structure alignment and visualization
    Copyright (C) 2011  Christopher M. Bruns

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
*/

/*! \mainpage MolTK: Python and C++ tools for protein sequence/structure alignment and visualization

\section whatismoltk What is MolTK?

There are three faces of MolTK; in order from easiest-to-use to most-powerful:

    -# MolTK is a computer application that allows the user to view and align molecular sequences and structures.
    -# moltk is a Python programming language module that allows the user to align and otherwise compute on molecular sequences and structures.
    -# moltk is a C++ API that allow the programmer to compute with molecular sequences and structures with high efficiency. 

\section resources MolTK Resources

    - Browse the MolTK home page at http://rotatingpenguin.com/moltk/
    - Download MolTK at http://code.google.com/p/moltk/downloads/list
    - Find questions and answers in the moltk-users forum at http://groups.google.com/group/moltk-users
    - Report issues and get source code at http://code.google.com/p/moltk/
    - Study the MolTK Python API at http://www.rotatingpenguin.com/moltk/api_python/python_api.html
    - Study the MolTK C++ API at http://www.rotatingpenguin.com/moltk/api_cxx/cxx_api.html

\section designgoals Design Goals of MolTK

    - Python programming language interactive environment for sequence/structure alignment that is just as easy to use as our (aging) dedicated alignment tool SEQUOIA.
    - Flexible architecture that makes it easy to experiment with custom alignment methods and scoring systems.
    - Consistent, well docomented API for both Python and C++ programmers.
    - Units-aware quantity type system. Thus alignment scores are not just numbers, they are information quantities with units of "bits". Atomic coordinates are not just numbers x, y, z, but are vector quantities with units of nanometers. Units-aware quantity types are an important part of scientific computing hygiene:
        - Converting a "quantity" to a raw number requires a "unit" to express the quantity in. This requires the user to pay attention to units at precisely the moment when knowing the unit is most important. The rest of the time is "just works", even if you are wrong about what the current units are!
        - This sort of type safety in scientific computing might help prevent errors such as, say, crashing $100 million orbiters into planets.
        - Type safety: Adding a length to a volume makes no sense, and will result in an error. Dividing a length by a time results in a velocity. 
    - Next-generation molecular sequence/structure viewer that adheres to our user interface principles. 

\section license MolTK License:

    Copyright (C) 2011  Christopher M. Bruns

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
*/

/*!
 *  The moltk namespace contains everything in the MolTK C++ API.
 */
namespace moltk {}

#include "moltk/Real.hpp"
#include "moltk/Biosequence.hpp"
#include "moltk/Aligner.hpp"
#include "moltk/PDBStructure.hpp"
#include "moltk/Vector3D.hpp"
#include "moltk/Rotation3D.hpp"
#include "moltk/RigidTransform.hpp"
