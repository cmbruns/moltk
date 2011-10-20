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

#include "moltk/Aligner.hpp"
#include "moltk/MatrixScorer.hpp"
#include <cassert>
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
    : scorer(NULL)
{init();}

/* virtual */
Aligner::~Aligner()
{
    clear_positions();
    clear_scorer();
}

void Aligner::init()
{
    clear_scorer();
    scorer = new MatrixScorer(MatrixScorer::get_blosum62_scorer());
    // gapOpenPenalty = 1.0 * bit; 
    // gapExtensionPenalty = 0.5 * bit;
    // m and n are lengths of input Biosequences
    // the PositionLists (seq1,seq2) and DpTable entries are actually +1 larger.
    m = seq1.size() - 1;
    n = seq2.size() - 1;
}

void Aligner::clear_scorer()
{
    delete scorer;
    scorer = NULL;
}

void Aligner::clear_positions() 
{
    for (size_t i = 0; i < seq1.size(); ++i) 
    {
        delete seq1[i];
        seq1[i] = NULL;
    }
    seq1.clear();
    for (size_t i = 0; i < seq2.size(); ++i) 
    {
        delete seq2[i];
        seq2[i] = NULL;
    }
    seq2.clear();
}

Alignment Aligner::align(const Alignment& s1, const Alignment& s2)
{
    clear_positions();
    target_alignment = s1;
    query_alignment = s2;
    // Fill seq1, seq2
    seq1.clear();
    // Create an extra Aligner::position at the very beginning, to hold left end gap data
    m = s1.get_number_of_columns();
    seq1 = scorer->create_target_positions(s1);

    seq2.clear();
    // Create an extra Aligner::position at the very beginning, to hold left end gap data
    n = s2.get_number_of_columns();
    seq2 = scorer->create_query_positions(s2);

    allocate_dp_table();
    initialize_dp_table();
    compute_recurrence();
    output_alignment = compute_traceback();
    return output_alignment;
}

void Aligner::allocate_dp_table()
{
    // TODO - small memory version allocates just 2 or 3 rows.
    dp_table.assign(m + 1, DpRow(n + 1));
}

void Aligner::initialize_dp_table()
{
    for (size_t i = 0; i < dp_table.size(); ++i)
        initialize_dp_row(i, dp_table[i]);
}

// single row initialization could come in handy during small memory alignment
void Aligner::initialize_dp_row(size_t rowIndex, DpRow& row)
{
    TargetPosition& p1 = *seq1[0];
    QueryPosition& p2 = *seq2[0];

    size_t i = rowIndex;
    // most rows only need the first element initialized
    row[0].f = numeric_limits<Real>::infinity() * bit;
    row[0].v = row[0].e = 
        -p1.get_gap_open_penalty() - ((double)i * p1.get_gap_extension_penalty());

    // ...but the top row gets full treatment
    if (rowIndex == 0) 
    {
        // upper left corner gets special treatment
        Cell& cornerCell = row[0];
        cornerCell.e = -p1.get_gap_open_penalty();
        cornerCell.f = -p2.get_gap_open_penalty();
        cornerCell.g = 0.0 * bit;
        cornerCell.v = 0.0 * bit;
        for (size_t j = 1; j < row.size(); ++j)
        {
            Cell& cell = row[j];
            cell.e = numeric_limits<Real>::infinity() * bit;
            cell.v = cell.f = 
                -p2.get_gap_open_penalty() - ((double)j * p2.get_gap_extension_penalty());
        }
    }
}

void Aligner::compute_cell_recurrence(int i, int j)
{
    const TargetPosition& p1 = *seq1[i];
    const QueryPosition& p2 = *seq2[j];
    Cell& cell = dp_table[i][j];
    const Cell& left = dp_table[i][j-1];
    const Cell& up = dp_table[i-1][j];
    const Cell& upLeft = dp_table[i-1][j-1];
    // This recurrence comes from Gusfield chapter 11.
    //  << "score " << i << ", " << j << " = " << p1.score(p2) << endl;
    cell.g = upLeft.v + p1.score(p2); // score...
    cell.e = std::max(left.e, 
                      (Information)(left.v - p1.get_gap_open_penalty()))
                 - p1.get_gap_extension_penalty();
    cell.f = std::max(up.f, 
                      (Information)(up.v - p2.get_gap_open_penalty()))
                 - p2.get_gap_extension_penalty();
    cell.v = cell.compute_v();
    //  << cell << endl;
}

void Aligner::compute_recurrence()
{
    for (size_t i = 1; i <= m; ++i)
        for (size_t j = 1; j <= n; ++j)
            compute_cell_recurrence(i, j);
}

Alignment Aligner::compute_traceback()
{
    // Start at lower right of dynamic programming matrix.
    Alignment::EString eString1;
    Alignment::EString eString2;
    int i = m;
    int j = n;
    Information alignmentScore = dp_table[i][j].v;
    // cout << "final alignment score = " << dp_table[i][j].v << endl;
    TracebackPointer tracebackPointer = dp_table[i][j].compute_traceback_pointer();
    while( (i > 0) || (j > 0) )
    {
        // cout << "traceback[" << i << "][" << j << "]" << endl;
        switch(tracebackPointer)
        {
        case TRACEBACK_UPLEFT:
            --i; --j;
            eString1.append_run(1);
            eString2.append_run(1);
            // cout << "upleft" << endl;
            break;
        case TRACEBACK_UP:
            --i;
            eString1.append_run(1);
            eString2.append_run(-1);
            // cout << "up" << endl;
            break;
        case TRACEBACK_LEFT:
            --j;
            eString1.append_run(-1);
            eString2.append_run(1);
            // cout << "left" << endl;
            break;
        default:
            cerr << "Traceback error!" << endl;
            assert(false);
            break;
        }
        tracebackPointer = dp_table[i][j].compute_traceback_pointer();
    }
    assert(i == 0);
    assert(j == 0);
    // OK, the sequences are actually backwards here
    eString1.reverse();
    eString2.reverse();
    // cerr << eString1 << eString2 << endl;
    // cerr << targetAlignment << queryAlignment;
    Alignment result = target_alignment.align(query_alignment, eString1, eString2);
    result.set_score(result.score() + alignmentScore);
    return result;
}

/* static */
const Aligner::Scorer& Aligner::get_default_scorer() {
    return MatrixScorer::get_blosum62_scorer();
}


////////////////////
// global methods //
////////////////////

