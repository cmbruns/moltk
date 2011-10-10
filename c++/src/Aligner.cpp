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
#include "moltk/MatrixScorer.h"
#include <cassert>
#include <algorithm> // reverse
#include <limits>

using namespace std;
using namespace moltk;
using moltk::units::bit;


///////////////////////////
// Aligner::Cell methods //
///////////////////////////

units::Information Aligner::Cell::compute_v() const
{
    TracebackPointer tp = compute_traceback_pointer();
    switch(tp)
    {
    case TRACEBACK_UPLEFT:
        return g;
    case TRACEBACK_LEFT:
        return e;
    default:
        return f;
    }
}

Aligner::TracebackPointer Aligner::Cell::compute_traceback_pointer() const
{
    if ( (g >= e) && (g >= f) )
        return TRACEBACK_UPLEFT;
    else if (e >= f)
        return TRACEBACK_LEFT;
    else
        return TRACEBACK_UP;
}


/////////////////////
// Aligner methods //
/////////////////////

Aligner::Aligner()
{init();}

void Aligner::init()
{
    scorer = new MatrixScorer(MatrixScorer::getBlosum62Scorer());
    // gapOpenPenalty = 1.0 * bit; 
    // gapExtensionPenalty = 0.5 * bit;
    // m and n are lengths of input Biosequences
    // the PositionLists (seq1,seq2) and DpTable entries are actually +1 larger.
    m = seq1.size() - 1;
    n = seq2.size() - 1;
}

Alignment Aligner::align(const Biosequence& s1, const Biosequence& s2)
{

    // Fill seq1, seq2
    seq1.clear();
    // Create an extra Aligner::position at the very beginning, to hold left end gap data
    m = s1.size();
    seq1.push_back(scorer->createPosition('*', true)); // position "-1"
    for(size_t i = 0; i < m - 1; ++i)
        seq1.push_back(scorer->createPosition(s1[i]));
    seq1.push_back(scorer->createPosition(s1[m - 1], true));

    seq2.clear();
    // Create an extra Aligner::position at the very beginning, to hold left end gap data
    n = s2.size();
    seq2.push_back(scorer->createPosition('*', true)); // position "-1"
    for(size_t i = 0; i < n - 1; ++i)
        seq2.push_back(scorer->createPosition(s2[i]));
    seq2.push_back(scorer->createPosition(s2[n - 1], true));

    allocate_dp_table();
    initialize_dp_table();
    compute_recurrence();
    alignment = compute_traceback();
    return alignment;
}

void Aligner::allocate_dp_table()
{
    // TODO - small memory version allocates just 2 or 3 rows.
    dpTable.assign(m + 1, DpRow(n + 1));
}

void Aligner::initialize_dp_table()
{
    for (size_t i = 0; i < dpTable.size(); ++i)
        initialize_dp_row(i, dpTable[i]);
}

// single row initialization could come in handy during small memory alignment
void Aligner::initialize_dp_row(size_t rowIndex, DpRow& row)
{
    Position& p1 = *seq1[0];
    Position& p2 = *seq2[0];

    size_t i = rowIndex;
    // most rows only need the first element initialized
    row[0].f = numeric_limits<Real>::infinity() * bit;
    row[0].v = row[0].e = 
        -p1.gapOpenPenalty() - ((double)i * p1.gapExtensionPenalty());

    // ...but the top row gets full treatment
    if (rowIndex == 0) {
        for (size_t j = 0; j < row.size(); ++j)
        {
            Cell& cell = row[j];
            cell.e = numeric_limits<Real>::infinity() * bit;
            cell.v = cell.f = 
                -p2.gapOpenPenalty() - ((double)j * p2.gapExtensionPenalty());
        }
    }
}

void Aligner::compute_cell_recurrence(int i, int j)
{
    const Position& p1 = *seq1[i];
    const Position& p2 = *seq2[j];
    Cell& cell = dpTable[i][j];
    const Cell& left = dpTable[i][j-1];
    const Cell& up = dpTable[i-1][j];
    const Cell& upLeft = dpTable[i-1][j-1];
    // This recurrence comes from Gusfield chapter 11.
    cell.g = upLeft.v + p1.score(p2); // score...
    cell.e = std::max(left.e, 
                      (Information)(left.v - p1.gapOpenPenalty()))
                 - p1.gapExtensionPenalty();
    cell.f = std::max(up.f, 
                      (Information)(up.v - p2.gapOpenPenalty()))
                 - p2.gapExtensionPenalty();
    cell.v = cell.compute_v();
}

void Aligner::compute_recurrence()
{
    for (size_t i = 1; i <= m; ++i)
        for (size_t j = 1; j <= n; ++j)
            compute_cell_recurrence(i, j);
}

Alignment Aligner::compute_traceback()
{
    // TODO - implement end gaps free version
    Alignment result;
    result.assign(2, std::string());

    // Start at lower right of dynamic programming matrix.
    int i = m;
    int j = n;
    // cout << "final alignment score = " << dpTable[i][j].v << endl;
    TracebackPointer tracebackPointer = dpTable[i][j].compute_traceback_pointer();
    while( (i > 0) || (j > 0) )
    {
        // cout << "traceback[" << i << "][" << j << "]" << endl;
        switch(tracebackPointer)
        {
        case TRACEBACK_UPLEFT:
            --i; --j;
            result[0].push_back( seq1[i+1]->getOneLetterCode() );
            result[1].push_back( seq2[j+1]->getOneLetterCode() );
            break;
        case TRACEBACK_UP:
            --i;
            result[0].push_back( seq1[i+1]->getOneLetterCode() );
            result[1].push_back( '-' );
            break;
        case TRACEBACK_LEFT:
            --j;
            result[0].push_back( '-' );
            result[1].push_back( seq2[j+1]->getOneLetterCode() );
            break;
        default:
            cerr << "Traceback error!" << endl;
            assert(false);
            break;
        }
        tracebackPointer = dpTable[i][j].compute_traceback_pointer();
    }
    assert(i == 0);
    assert(j == 0);
    // OK, the sequences are actually backwards here
    for (size_t i = 0; i < result.size(); ++i)
        std::reverse(result[i].begin(), result[i].end());
    return result;
}

/* static */
const Aligner::Scorer& Aligner::getDefaultScorer() {
    return MatrixScorer::getBlosum62Scorer();
}


////////////////////
// global methods //
////////////////////

