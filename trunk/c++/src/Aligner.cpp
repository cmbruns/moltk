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

#include "moltk/Aligner.hpp"
#include "moltk/MatrixScorer.hpp"
#include "moltk/EString.hpp"
#include <cassert>
#include <limits>

using namespace std;
using namespace moltk;
using moltk::units::bit;


/////////////////////
// Aligner methods //
/////////////////////

Aligner::Aligner()
    : scorer(NULL)
{init();}

/* virtual */
Aligner::~Aligner()
{
    // clear_positions();
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
    m = test_table.target_positions.size() - 1;
    n = test_table.query_positions.size() - 1;
}

void Aligner::clear_scorer()
{
    delete scorer;
    scorer = NULL;
}

/*
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
*/

Alignment Aligner::align(const Alignment& s1, const Alignment& s2)
{
    test_table.clear_positions();
    target_alignment = s1;
    query_alignment = s2;
    // Fill seq1, seq2
    // seq1.clear();
    // Create an extra Aligner::position at the very beginning, to hold left end gap data
    m = s1.get_number_of_columns();
    test_table.target_positions = scorer->create_target_positions(s1);

    // seq2.clear();
    // Create an extra Aligner::position at the very beginning, to hold left end gap data
    n = s2.get_number_of_columns();
    test_table.query_positions = scorer->create_query_positions(s2);

    dp::AlignmentResult<Information> alignment_result =
            test_table.align();
    output_alignment =
            target_alignment.align(
                    query_alignment,
                    alignment_result.eString1,
                    alignment_result.eString2);
    output_alignment.set_score(output_alignment.get_score() + alignment_result.score);
    return output_alignment;
}

Alignment Aligner::compute_traceback()
{
    dp::AlignmentResult<Information> alignment_result = test_table.compute_traceback();
    Alignment result =
            target_alignment.align(
                    query_alignment,
                    alignment_result.eString1,
                    alignment_result.eString2);
    result.set_score(result.get_score() + alignment_result.score);
    return result;
}

/* static */
const Aligner::Scorer& Aligner::get_default_scorer() {
    return MatrixScorer::get_blosum62_scorer();
}

/* static */
Aligner& Aligner::get_shared_aligner()
{
    static std::auto_ptr<Aligner> shared_aligner(NULL);
    if (!shared_aligner.get())
        shared_aligner.reset( new Aligner() );
    return *shared_aligner.get();
}


////////////////////
// global methods //
////////////////////

/*
 * global align() methods helps get SEQUOIA-like conciseness in python.
 */
Alignment moltk::align(const Alignment& target_alignment, const Alignment& query_alignment)
{
    return Aligner::get_shared_aligner().align(target_alignment, query_alignment);
}
