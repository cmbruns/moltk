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

#include "moltk/Alignment.hpp"
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
    append_sequence(seq);
}

/* implicit */ 
Alignment::Alignment(const std::string& s)
 : m_score(0.0 * moltk::units::bit)
{
    load_string(s);
}

/* implicit */ 
Alignment::Alignment(const char* str)
 : m_score(0.0 * moltk::units::bit)
{
    std::string s(str);
    load_string(s);
}

size_t Alignment::get_number_of_columns() const
{
    if (rows.size() == 0) return 0;
    return rows[0].e_string.total_length();
}

void Alignment::load_string(const std::string& s)
{
    Biosequence b(s);
    append_sequence(b);
}

Alignment& Alignment::append_sequence(const Biosequence& seq)
{
    sequences.push_back(seq);
    Row row = {
        SequenceList,
        0,
        1.0,
        EString().append_run(seq.size()) };
    rows.push_back(row);
    return *this;
}

/* virtual */
void moltk::Alignment::print_string(std::ostream& os) const
{
    for (size_t rowIx = 0; rowIx < rows.size(); ++rowIx) 
    {
        const Row& row = rows[rowIx];
        const BaseBiosequence* seq;
        if (row.list == SequenceList)
            seq = &sequences[row.list_index];
        else
            seq = &structures[row.list_index];
        EString::const_iterator i = row.e_string.begin();
        while(i != row.e_string.end()) {
            int resIx = *i;
            if (resIx < 0)
                os << '-'; // gap
            else
                os << seq->get_residue(resIx).get_one_letter_code();
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
        newRow.e_string = e1 * newRow.e_string;
        if (row.list == SequenceList) {
            result.sequences.push_back(sequences[row.list_index]);
            assert(result.sequences.size() - 1 == newRow.list_index);
        }
        else {
            assert(row.list == StructureList);
            result.structures.push_back(structures[row.list_index]);
            assert(result.structures.size() - 1 == newRow.list_index);
        }
        result.rows.push_back(newRow);
    }
    // Add sequences from second alignment
    for (size_t r = 0; r < a2.rows.size(); ++r)
    {
        const Row& row = a2.rows[r];
        Row newRow(row);
        newRow.e_string = e2 * newRow.e_string;
        if (row.list == SequenceList) {
            result.sequences.push_back(a2.sequences[row.list_index]);
            newRow.list_index = result.sequences.size() - 1;
        }
        else {
            assert(row.list == StructureList);
            result.structures.push_back(a2.structures[row.list_index]);
            newRow.list_index = result.structures.size() - 1;
        }
        result.rows.push_back(newRow);
    }
    result.set_score(a1.score() + a2.score());

    return result;
}


/////////////////////////////////////
// EString::const_iterator methods //
/////////////////////////////////////

Alignment::EString::const_iterator::const_iterator(const std::vector<int>& runsParam
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

Alignment::EString::const_iterator& Alignment::EString::const_iterator::operator++()
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

int Alignment::EString::const_iterator::operator*() const
{
    if (run_index >= runs->size()) return -1; // end()
    if ((*runs)[run_index] < 0) return -1; // gap
    return sequence_index; // current sequence position
}

bool Alignment::EString::const_iterator::operator!=(const const_iterator& rhs)
{
    if (run_index != rhs.run_index) return true;
    if (position_index != rhs.position_index) return true;
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

Alignment::EString Alignment::EString::operator*(const EString& rhs) const
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

Alignment::EString& Alignment::EString::append_run(int run)
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

size_t Alignment::EString::ungapped_length() const {
    return m_ungapped_length;
}

size_t Alignment::EString::total_length() const {
    return m_total_length;
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


