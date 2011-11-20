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
#define BOOST_TEST_MODULE AlignmentUnitTest
#include "moltk/Alignment.hpp"
#include "moltk/Aligner.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/debug.hpp>
#include <iostream>

using namespace std;
using namespace moltk;
using moltk::units::bit;

BOOST_AUTO_TEST_CASE( test_alignment )
{
    boost::debug::detect_memory_leaks(false);
    // TODO - test calc_explicit_sum_of_pairs_score
    Alignment a("AAASGD");
    const MatrixScorer& scorer = Aligner::get_shared_aligner().get_scorer();
    // Sum of pairs should be zero with just one sequence - OK
    BOOST_CHECK_EQUAL(a.get_score(), scorer.calc_explicit_sum_of_pairs_score(a));
    BOOST_CHECK_EQUAL(0.0 * bit, scorer.calc_explicit_sum_of_pairs_score(a));
    // Trivial sum of pairs with one letter, presumably no gaps
    Alignment a1("S");
    Alignment a2 = align(a1, a1);
    cout << Aligner::get_shared_aligner().test_table << endl;
    BOOST_CHECK_EQUAL(2.0 * bit, a2.get_score());
    BOOST_CHECK_EQUAL(a2.get_score(), scorer.calc_explicit_sum_of_pairs_score(a2));
}

