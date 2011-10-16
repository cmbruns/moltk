#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EStringUnitTest
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include "moltk/Alignment.h"

using namespace moltk;
typedef moltk::Alignment::EString EString;

BOOST_AUTO_TEST_CASE( test_estring_equality )
{
    EString s1;
    EString s2;
    BOOST_CHECK_EQUAL( s1, s2 );
    EString s3;
    s3.appendRun(2);
    BOOST_CHECK_NE( s1, s3 );
    EString s4;
    s4.appendRun(2);
    BOOST_CHECK_EQUAL( s3, s4 );
    EString s5;
    s5.appendRun(2);
    s5.appendRun(-1);
    BOOST_CHECK_NE(s3, s5);
}

