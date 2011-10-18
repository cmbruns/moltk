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
 : m_score(0.0 * moltk::units::bit)
{
    appendSequence(seq);
}

/* implicit */ 
Alignment::Alignment(const std::string& s)
 : m_score(0.0 * moltk::units::bit)
{
    loadString(s);
}

/* implicit */ 
Alignment::Alignment(const char* str)
 : m_score(0.0 * moltk::units::bit)
{
    std::string s(str);
    loadString(s);
}

size_t Alignment::getNumberOfColumns() const
{
    if (rows.size() == 0) return 0;
    return rows[0].eString.totalLength();
}

void Alignment::loadString(const std::string& s)
{
    Biosequence b(s);
    appendSequence(b);
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
        EString::const_iterator i = row.eString.begin();
        while(i != row.eString.end()) {
            int resIx = *i;
            if (resIx < 0)
                os << '-'; // gap
            else
                os << seq->getResidue(resIx).get_one_letter_code();
            ++i;
        }
        os << endl;
    }
}

Alignment Alignment::align(const Alignment& a2, const EString& e1, const EString& e2) const
{
    const Alignment& a1 = *this;
    // cerr << *this;
    // cerr << a2;
    // cerr << e1 << endl;
    // cerr << e2 << endl;
    Alignment result;
    // Add sequences from first alignment
    for (size_t r = 0; r < rows.size(); ++r)
    {
        const Row& row = rows[r];
        Row newRow(row);
        newRow.eString = e1 * newRow.eString;
        if (row.list == SequenceList) {
            result.sequences.push_back(sequences[row.listIndex]);
            assert(result.sequences.size() - 1 == newRow.listIndex);
        }
        else {
            assert(row.list == StructureList);
            result.structures.push_back(structures[row.listIndex]);
            assert(result.structures.size() - 1 == newRow.listIndex);
        }
        result.rows.push_back(newRow);
    }
    // Add sequences from second alignment
    for (size_t r = 0; r < a2.rows.size(); ++r)
    {
        const Row& row = a2.rows[r];
        Row newRow(row);
        newRow.eString = e2 * newRow.eString;
        if (row.list == SequenceList) {
            result.sequences.push_back(a2.sequences[row.listIndex]);
            newRow.listIndex = result.sequences.size() - 1;
        }
        else {
            assert(row.list == StructureList);
            result.structures.push_back(a2.structures[row.listIndex]);
            newRow.listIndex = result.structures.size() - 1;
        }
        result.rows.push_back(newRow);
    }
    result.setScore(a1.score() + a2.score());

    return result;
}


/////////////////////////////////////
// EString::const_iterator methods //
/////////////////////////////////////

Alignment::EString::const_iterator::const_iterator(const std::vector<int>& runsParam
        , int runIndexParam
        , int positionIndexParam)
        : runs(&runsParam)
        , runIndex(runIndexParam)
        , positionIndex(positionIndexParam)
        , sequenceIndex(-1) // default to gap start
{
    if ( (runs->size() > runIndex) && ((*runs)[runIndex] > 0) )
        sequenceIndex = 0; // EString starts with non-gap
}

Alignment::EString::const_iterator& Alignment::EString::const_iterator::operator++()
{
    ++positionIndex;
    if (positionIndex >= std::abs((*runs)[runIndex]))
    {
        positionIndex = 0;
        ++runIndex;
    }
    if ( (runIndex < runs->size())
        && ( (*runs)[runIndex] > 0 ) )
    {
        ++sequenceIndex;
    }
    return *this;
}

int Alignment::EString::const_iterator::operator*() const
{
    if (runIndex >= runs->size()) return -1; // end()
    if ((*runs)[runIndex] < 0) return -1; // gap
    return sequenceIndex; // current sequence position
}

bool Alignment::EString::const_iterator::operator!=(const const_iterator& rhs)
{
    if (runIndex != rhs.runIndex) return true;
    if (positionIndex != rhs.positionIndex) return true;
    return false;
}

bool Alignment::EString::const_iterator::operator==(const const_iterator& rhs)
{
    return ! (*this != rhs);
}


/////////////////////
// EString methods //
/////////////////////

Alignment::EString::EString()
    : m_ungappedLength(0)
    , m_totalLength(0)
{}

Alignment::EString Alignment::EString::operator*(const EString& rhs) const
{
    // cerr << "EString::operator*()" << *this << rhs << endl;
    const EString & lhs = *this;
    assert(lhs.ungappedLength() == rhs.ungappedLength());
    EString result;
    // seqIx1 is current ungapped sequence position in EString 1
    size_t seqIx1 = -1;
    size_t seqIx2 = -1;
    // eIx1 is current run index in EString 1
    size_t eIx1 = 0;
    size_t eIx2 = 0;
    // run 1 is the remaining portion of the current run in EString 1
    int run1 = 0;
    int run2 = 0;
    while (    (eIx1 < lhs.runs.size())
            || (eIx2 < rhs.runs.size())
            || (run1 != 0)
            || (run2 != 0)
          )
    {
        // With each loop, zero out either run1 or run2.
        // Populate empty run values.
        if ( (run1 == 0) && (eIx1 < lhs.runs.size()) ) {
            run1 = lhs.runs[eIx1];
            ++eIx1;
        }
        if ( (run2 == 0) && (eIx2 < rhs.runs.size()) ) {
            run2 = rhs.runs[eIx2];
            ++eIx2;
        }
        if ( (run1 > 0) && (run2 > 0) ) // both non-gaps, consume min
        {
            int min = std::min(run1, run2);
            result.appendRun(min);
            run1 -= min;
            run2 -= min;
            seqIx1 += min;
            seqIx2 += min;
        }
        // at least one EString with gaps; consume them.
        if (run1 < 0) {
            result.appendRun(run1);
            run1 = 0;
        }
        if (run2 < 0) {
            result.appendRun(run2);
            run2 = 0;
        }
        assert(seqIx1 == seqIx2);
    }
    assert(result.ungappedLength() == lhs.ungappedLength());
    assert(seqIx1 == result.ungappedLength() - 1);
    assert(seqIx2 == result.ungappedLength() - 1);
    return result;
}

Alignment::EString& Alignment::EString::appendRun(int run)
{
    if (run == 0) return *this; // run of nothing
    m_totalLength += std::abs(run);
    if (run > 0)
        m_ungappedLength += run;
    if (runs.size() == 0) { // first run
        runs.push_back(run);
        return *this;
    }
    if ( (run * runs.back()) > 0 ) { // same sign: combine
        runs.back() += run;
        return *this;
    }
    // different sign: append
    runs.push_back(run);
    return *this;
}

size_t Alignment::EString::ungappedLength() const {
    return m_ungappedLength;
}

size_t Alignment::EString::totalLength() const {
    return m_totalLength;
}

Alignment::EString::const_iterator Alignment::EString::begin() const
{
    return const_iterator(runs, 0, 0);
}

Alignment::EString::const_iterator Alignment::EString::end() const
{
    return const_iterator(runs, runs.size(), 0);
}

void Alignment::EString::reverse()
{
    // cout << "forward" << *this;
    std::reverse(runs.begin(), runs.end());
    // cout << "reverse" << *this << endl;
}


