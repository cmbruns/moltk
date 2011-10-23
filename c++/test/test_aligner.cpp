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
using namespace std;

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
}
