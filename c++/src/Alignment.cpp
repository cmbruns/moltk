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

#include "moltk/Alignment.h"
#include <cassert>
#include <sstream>

using namespace moltk;
using namespace std;


///////////////////////
// Alignment methods //
///////////////////////

/* implicit */
Alignment::Alignment(const Biosequence& seq) 
{
    appendSequence(seq);
}

/* implicit */ 
Alignment::Alignment(const std::string& s)
{
    loadString(s);
}

/* implicit */ 
Alignment::Alignment(const char* str)
{
    std::string s(str);
    loadString(s);
}

void Alignment::loadString(const std::string& s)
{
    stringstream ss(stringstream::in | stringstream::out);
    ss << s;
    while (ss.good())
    {
        Biosequence b;
        ss >> b;
        if (b.size() > 0)
            appendSequence(b);
    }
}

void Alignment::appendSequence(const Biosequence& seq)
{
    if (0 == size())
        assign(seq.size(), Column());
    assert(size() == seq.size());
    for(size_t r = 0; r < size(); ++r)
        (*this)[r].push_back(new Biosequence::Residue(seq[r]));
}

/* virtual */
void moltk::Alignment::printString(std::ostream& os) const
{
    if (size() < 1) return;
    int nRows = (*this)[0].size();
    int nCols = size();
    for (int r = 0; r < nRows; ++r) {
        for (int c = 0; c < nCols; ++c) {
            os << (*this)[c][r]->getOneLetterCode();
        }
        os << endl;
    }
}

