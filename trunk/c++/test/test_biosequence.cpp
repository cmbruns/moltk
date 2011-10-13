#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BiosequenceUnitTest
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include "moltk/Biosequence.h"

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

