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
#define BOOST_TEST_MODULE AlignerUnitTest
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/debug.hpp>
#include "moltk.hpp"
#include <iostream>
#include <cassert>


using namespace std;
using namespace moltk;
using moltk::units::bit;
using namespace boost;

BOOST_AUTO_TEST_CASE( test_aligner )
{
    boost::debug::detect_memory_leaks(false);
    // boost::debug::break_memory_alloc(566);
    Aligner aligner;
    aligner.get_scorer().set_end_gap_factor(0.0);

    Alignment a1 = aligner.align("W", "W");
    cout << a1 << a1.get_score() << endl;
    cout << aligner.test_table << endl;
    BOOST_CHECK_EQUAL(a1.get_score(), 5.5 * bit);

    aligner.get_scorer().set_end_gap_factor(1.0);
    Alignment a = aligner.align("QS", "SD");
    cout << a << a.get_score() << endl;
    cout << aligner.test_table << endl;
    BOOST_CHECK_EQUAL(a.get_score(), 0.0 * bit);
    aligner.get_scorer().set_end_gap_factor(0.0);
    a = aligner.align("QS", "SD");
    cout << a << a.get_score() << endl;
    cout << aligner.test_table << endl;
    BOOST_CHECK_EQUAL(a.get_score(), 2.0 * bit);

    Biosequence seq1("AACDEF");
    Biosequence seq2("ACEFG");
    Alignment a2 = aligner.align(seq1, seq2);
    cout << a2 << a2.get_score() << endl;
}

