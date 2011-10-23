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

#include "moltk/EString.hpp"
#include <cassert>
#include <sstream>
#include <algorithm> // reverse

using namespace moltk;
using namespace std;


/////////////////////////////////////
// EString::const_iterator methods //
/////////////////////////////////////

EString::const_iterator::const_iterator(const std::vector<int>& runsParam
        , int runIndexParam
        , int positionIndexParam)
        : runs(&runsParam)
        , run_index(runIndexParam)
        , position_index(positionIndexParam)
        , sequence_index(-1) // default to gap start
{
    if ( (runs->size() > run_index) && ((*runs)[run_index] > 0) )
        sequence_index = 0; // EString starts with non-gap
}

EString::const_iterator& EString::const_iterator::operator++()
{
    ++position_index;
    if (position_index >= std::abs((*runs)[run_index]))
    {
        position_index = 0;
        ++run_index;
    }
    if ( (run_index < runs->size())
        && ( (*runs)[run_index] > 0 ) )
    {
        ++sequence_index;
    }
    return *this;
}

int EString::const_iterator::operator*() const
{
    if (run_index >= runs->size()) return -1; // end()
    if ((*runs)[run_index] < 0) return -1; // gap
    return sequence_index; // current sequence position
}

bool EString::const_iterator::operator!=(const const_iterator& rhs)
{
    if (run_index != rhs.run_index) return true;
    if (position_index != rhs.position_index) return true;
    return false;
}

bool EString::const_iterator::operator==(const const_iterator& rhs)
{
    return ! (*this != rhs);
}


/////////////////////
// EString methods //
/////////////////////

EString::EString()
    : m_ungapped_length(0)
    , m_total_length(0)
{}

// Helpers for EString multiplication.
// Finite state machine to handle tricky cases of EString composition
// Numbering is abitrary to match our notes.
enum EStringMulState {
    STATE_1_ZZ, // both run nibbles zero
    STATE_2_PZ,
    STATE_3_NZ,
    STATE_4_ZP,
    STATE_5_PP, // both run nibbles positive
    STATE_6_NP,
    STATE_7_ZN,
    STATE_8_PN,
    STATE_9_NN // both run nibbles negative
};

EStringMulState getState(int run1, int run2)
{
    if (run1 == 0) {
        if (run2 == 0) return STATE_1_ZZ;
        else if (run2 > 0) return STATE_4_ZP;
        else {assert (run2 < 0); return STATE_7_ZN;}
    }
    else if (run1 > 0) {
        if (run2 == 0) return STATE_2_PZ;
        else if (run2 > 0) return STATE_5_PP;
        else {assert (run2 < 0); return STATE_8_PN;}
    }
    else {
        assert(run1 < 0);
        if (run2 == 0) return STATE_3_NZ;
        else if (run2 > 0) return STATE_6_NP;
        else {assert (run2 < 0); return STATE_9_NN;}
    }
    assert(false); // should not get here
}

EString EString::operator*(const EString& rhs) const
{
    // cerr << "EString::operator*()" << *this << rhs << endl;
    const EString & lhs = *this;
    assert(lhs.ungapped_length() == rhs.total_length());
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
    EStringMulState state = getState(run1, run2);
    assert(state == STATE_1_ZZ);
    while ((eIx1 < lhs.runs.size()) 
        || (eIx2 < rhs.runs.size())
        || (run1 != 0)
        || (run2 != 0)
        )
    {
        int min;
        switch(state) 
        {
        case STATE_1_ZZ:
            // both nibbles zero.
            // populate each with a new bite.
            // take a bite of string 1
            if (eIx1 < lhs.runs.size()) {
                run1 = lhs.runs[eIx1];
                ++eIx1;
            }
            // take a bite of string 2
            if (eIx2 < rhs.runs.size()) {
                run2 = rhs.runs[eIx2];
                ++eIx2;
            }
            state = getState(run1, run2);
            break;
        case STATE_2_PZ:
            // take a bite of string 2
            if (eIx2 < rhs.runs.size()) {
                run2 = rhs.runs[eIx2];
                ++eIx2;
            }
            state = getState(run1, run2);
            break;
        case STATE_5_PP:
            min = std::min(run1, run2);
            result << min;
            run1 -= min;
            run2 -= min;
            seqIx1 += min;
            seqIx2 += min;
            state = getState(run1, run2);
            break;
        case STATE_4_ZP:
        case STATE_7_ZN:
            // take a bite of string 1
            if (eIx1 < lhs.runs.size()) {
                run1 = lhs.runs[eIx1];
                ++eIx1;
            }
            state = getState(run1, run2);
            break;
        case STATE_8_PN:
            // minimum absolute value
            min = std::min(std::abs(run1), std::abs(run2));            
            result << -min; // add some gaps
            run1 -= min;
            run2 += min;
            state = getState(run1, run2);
            break;
        case STATE_3_NZ:
        case STATE_6_NP:
        case STATE_9_NN:
            // First nibble negative.
            // Clear first nibble.
            result << run1;
            run1 = 0;
            state = getState(run1, run2);
            break;
        default:
            assert(false);
            break;
        }
        assert(seqIx1 == seqIx2);
    }
    state = getState(run1, run2);
    assert(state == STATE_1_ZZ);
    assert(result.ungapped_length() == rhs.ungapped_length());
    assert(result.total_length() == lhs.total_length());
    assert(seqIx1 == result.ungapped_length() - 1);
    assert(seqIx2 == result.ungapped_length() - 1);
    return result;
}

EString& EString::append_run(int run)
{
    if (run == 0) return *this; // run of nothing
    m_total_length += std::abs(run);
    if (run > 0)
        m_ungapped_length += run;
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

size_t EString::ungapped_length() const {
    return m_ungapped_length;
}

size_t EString::total_length() const {
    return m_total_length;
}

EString::const_iterator EString::begin() const
{
    return const_iterator(runs, 0, 0);
}

EString::const_iterator EString::end() const
{
    return const_iterator(runs, runs.size(), 0);
}

void EString::reverse()
{
    // cout << "forward" << *this;
    std::reverse(runs.begin(), runs.end());
    // cout << "reverse" << *this << endl;
}
