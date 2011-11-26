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

// #define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BiosequenceUnitTest
#include "moltk/Aligner.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/debug.hpp>
#include <iostream>

using namespace moltk;
using moltk::units::Information;
using moltk::units::bit;
using namespace std;

// Alignment with internal gaps but no new gaps
Information test_matchy_alignment_score(const MatrixScorer& scorer) {
    Alignment a1, a2;
    a1.load_fasta_string(
        ">1\n"
        "C--PW\n"
        ">2\n"
        "CHFPW\n"
        );
    a2.load_fasta_string(
        ">3\n"
        "----W\n"
        ">4\n"
        "CH-P-\n"
        ">5\n"
        "-HF--\n"
        ">6\n"
        "C-FPW\n"
        ">7\n"
        "CHFPW\n"
        );
    Alignment a3 = align(a1, a2);
    cout << a3 << endl;
    // We expect this net_score to equal the alignment score
    Information net_score = 
        scorer.calc_explicit_sum_of_pairs_score(a3)
        - scorer.calc_explicit_sum_of_pairs_score(a1)
        - scorer.calc_explicit_sum_of_pairs_score(a2);
    cout << net_score << endl;
    cout << a3.get_score() << endl;
    BOOST_CHECK_EQUAL(net_score, a3.get_score());
    return a3.get_score();
}

BOOST_AUTO_TEST_CASE( test_aligner )
{
    boost::debug::detect_memory_leaks(false);
    Aligner a;
    // Sequoia 3 sequence alignment paradigm
    Alignment seq1 = load_fasta("sequences/sod1_Hsap.fasta");
    Alignment seq2 = load_fasta("sequences/sod_Dmel.fasta");
    Alignment al1 = align(seq1, seq2);
    cout << al1 << endl;
    cout << seq1 << endl;
    Alignment seq3 = load_fasta("sequences/sod1_Scer.fasta");
    Alignment al2 = align(al1, seq3);
    cout << al2 << endl;
    Alignment al3 = align(seq3, al1);
    cout << al3 << endl;

    // Test for alignment of alignments
    // Gaps are arranged to efficiently cover all pair interactions (-X XX, -X -X, etc)
    MatrixScorer& scorer = Aligner::get_shared_aligner().get_scorer();
    // Start by debugging match score only
    scorer.set_default_gap_open_score(0.0 * bit);
    scorer.set_default_gap_extension_score(0.0 * bit);
    Information score = test_matchy_alignment_score(scorer); // this works 102b
    BOOST_CHECK_EQUAL(102.0 * bit, score);
    // Add in gap extension score
    scorer.set_default_gap_extension_score(-0.5 * bit);
    scorer.set_end_gap_factor(1.0);
    score = test_matchy_alignment_score(scorer); // works with penalized end gaps 91b
    BOOST_CHECK_EQUAL(91.0 * bit, score);
    scorer.set_end_gap_factor(0.0);
    score = test_matchy_alignment_score(scorer); // works 98b
    BOOST_CHECK_EQUAL(98.0 * bit, score);
    // TODO - add in gap opening score
    scorer.set_default_gap_open_score(-8.0 * bit);
    scorer.set_end_gap_factor(1.0);
    score = test_matchy_alignment_score(scorer);
}
