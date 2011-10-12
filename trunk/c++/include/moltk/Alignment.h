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

#ifndef MOLTK_ALIGN_ALIGNMENT_H
#define MOLTK_ALIGN_ALIGNMENT_H

#include <iostream>
#include <string>
#include <vector>
#include "moltk/Printable.h"
#include "moltk/Biosequence.h"

namespace moltk {

class AlignmentColumn : public std::vector<BaseBiosequence::BaseResidue*>
{
public:
    AlignmentColumn operator+(const AlignmentColumn& rhs) const;
    virtual ~AlignmentColumn() 
    {
        for(int r = 0; r < size(); ++r) {
            delete (*this)[r];
            (*this)[r] = NULL;
        }
    }
};

class Alignment : public moltk::Printable, public std::vector<AlignmentColumn>
{
public:
    typedef AlignmentColumn Column;

public:
    Alignment() {}
    /* implicit */ Alignment(const Biosequence&);
    /* implicit */ Alignment(const std::string&);
    /* implicit */ Alignment(const char*);
    virtual ~Alignment() {}
    virtual void print_to_stream(std::ostream& os) const;
};

// std::ostream& operator<<(std::ostream& os, const moltk::Alignment& p);

} // namespace moltk


#endif // MOLTK_ALIGN_ALIGNMENT_H
