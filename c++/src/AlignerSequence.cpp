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
    
    For questions contact: cmbruns@rotatingpenguin.com
*/

#include "moltk/Aligner.h"

namespace moltk {

Aligner::PositionList::PositionList(const PositionList& rhs)
{
    *this = rhs;
}

Aligner::PositionList& Aligner::PositionList::operator=(const PositionList& rhs)
{
    if (this == &rhs) return *this;
    assign(rhs.size(), NULL);
    for (size_t i = 0; i < size(); ++i) {
        (*this)[i] = rhs[i]->clone();
    }
    return *this;
}

/* virtual */
Aligner::PositionList::~PositionList()
{
    for (size_t i = 0; i < size(); ++i) {
        delete (*this)[i];
        (*this)[i] = NULL;
    }
}

} // namespace moltk

