#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE BiosequenceUnitTest
#include <boost/test/unit_test.hpp>
#include <boost/lexical_cast.hpp>
#include "moltk/PdbStructure.h"

using namespace moltk;

BOOST_AUTO_TEST_CASE( test_pdb_atom )
{
    PDBStructure::Atom a;
}

