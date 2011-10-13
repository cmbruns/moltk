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

Alignment& Alignment::appendSequence(const Biosequence& seq)
{
    sequences.push_back(seq);
    Row row = {
        SequenceList,
        0,
        1.0,
        EString().appendRun(seq.size()) };
    rows.push_back(row);
    return *this;
}

/* virtual */
void moltk::Alignment::printString(std::ostream& os) const
{
    for (size_t rowIx = 0; rowIx < rows.size(); ++rowIx) 
    {
        const Row& row = rows[rowIx];
        const BaseBiosequence* seq;
        if (row.list == SequenceList)
            seq = &sequences[row.listIndex];
        else
            seq = &structures[row.listIndex];
        for (size_t resIx = 0; resIx < seq->getNumberOfResidues(); ++resIx)
        {
            const BaseBiosequence::BaseResidue& residue = seq->getResidue(resIx);
            os << residue.getOneLetterCode();
        }
        os << endl;
    }
}

