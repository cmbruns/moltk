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
#define BOOST_TEST_MODULE EStringUnitTest
#include "moltk/EString.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>

using namespace moltk;

BOOST_AUTO_TEST_CASE( test_estring_equality )
{
    EString s1;
    EString s2;
    BOOST_CHECK_EQUAL( s1, s2 );
    EString s3;
    s3.append_run(2);
    BOOST_CHECK_NE( s1, s3 );
    EString s4;
    s4.append_run(2);
    BOOST_CHECK_EQUAL( s3, s4 );
    EString s5;
    s5.append_run(2);
    s5.append_run(-1);
    BOOST_CHECK_NE(s3, s5);
    BOOST_CHECK_EQUAL(
        EString(),
        EString());
    BOOST_CHECK_NE(
        EString() << 2,
        EString());
    // example from Edgar paper
    BOOST_CHECK_EQUAL(
        (EString()<<1<<-1<<5) * (EString()<<3<<-1<<2),
        EString()<<1<<-1<<2<<-1<<2);
    // tests from MUSCLE estring.cpp
    BOOST_CHECK_EQUAL(
        (EString()<<1<<-1<<2) * (EString()<<-1<<1<<-1<<0),
        EString()<<-2<<1<<-1);
    BOOST_CHECK_EQUAL(
        (EString()<<1<<-1<<3<<-1<<1) * (EString()<<2<<-1<<2),
        EString()<<1<<-1<<1<<-1<<1<<-1<<1);
    BOOST_CHECK_EQUAL(
        (EString()<<2<<-1<<2) * (EString()<<-1<<3),
        EString()<<-1<<1<<-1<<2);
    BOOST_CHECK_EQUAL(
        (EString()<<4) * (EString()<<-1<<1<<-1<<1),
        EString()<<-1<<1<<-1<<1);
    BOOST_CHECK_EQUAL(
        (EString()<<4) * (EString()<<1<<-1<<1<<-1),
        EString()<<1<<-1<<1<<-1);
    BOOST_CHECK_EQUAL(
        (EString()<<-1<<4<<-1) * (EString()<<1<<-1<<1<<-1),
        EString()<<-1<<1<<-1<<1<<-2);
}

