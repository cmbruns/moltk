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
#define BOOST_TEST_MODULE GappingUnitTest
#include "moltk/Aligner.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/debug.hpp>
#include <iostream>

using namespace moltk;
using moltk::units::Information;
using moltk::units::bit;
using namespace std;

BOOST_AUTO_TEST_CASE( test_gaps )
{
    boost::debug::detect_memory_leaks(false);
    Aligner& aligner = Aligner::get_shared_aligner();
    MatrixScorer& scorer = aligner.get_scorer();
    // Choose scores that will make it easy to see score components
    scorer.set_default_gap_extension_score(-0.07 * bit);
    scorer.set_default_gap_open_score(-0.30 * bit);
    scorer.set_end_gap_factor(1.0);

    {
        Alignment a1, a2;
        a1.load_fasta_string(
            ">1\n-C\n"
            );
        a2.load_fasta_string(
            ">3\nAC\n"
            );
        Alignment a3 = align(a1, a2);
        cout << a3 << endl;
        Information net_score = 
            scorer.calc_explicit_sum_of_pairs_score(a3)
            - scorer.calc_explicit_sum_of_pairs_score(a1)
            - scorer.calc_explicit_sum_of_pairs_score(a2);
        cout << net_score << endl;
        cout << a3.get_score() << endl;
        cout << aligner.test_table << endl;
        BOOST_CHECK_EQUAL(net_score, a3.get_score());
    }
}
