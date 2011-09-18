#include "chem/pdb.h"
#include <fstream>
#include <iostream>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#endif

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

using namespace std;
using namespace boost::iostreams;

int main()
{
    ifstream pdbFile("structures/1FND.pdb.gz", ios_base::in | ios_base::binary);
	filtering_streambuf<input> in;
	in.push(gzip_decompressor());
	in.push(pdbFile);
	boost::iostreams::copy(in, cout);
}
