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
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include "moltk/Biosequence.hpp"

using namespace moltk;

BOOST_AUTO_TEST_CASE( test_biosequence_equality )
{
    BOOST_CHECK_EQUAL( 1 + 1, 2 );
    Biosequence s1;
    Biosequence s2;
    BOOST_CHECK_EQUAL( s1, s2 );
    Biosequence s3("ACDEFG");
    BOOST_CHECK_NE( s1, s3 );
    Biosequence s4("ACDEFG");
    BOOST_CHECK_EQUAL( s3, s4 );
    Biosequence s5("ACDEF");
    BOOST_CHECK_NE(s3, s5);
    BOOST_CHECK_EQUAL( boost::lexical_cast<std::string>(s5), "ACDEF" );
}

