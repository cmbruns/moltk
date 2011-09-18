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

int main(int argc, char* argv[])
{
    if (argc < 2) 
        return 1; // file name argument missing
    const char* pdbFileName = argv[1];
    ifstream pdbFile(pdbFileName, ios_base::in | ios_base::binary);
    if (! pdbFile.good())
        return 1;
	filtering_streambuf<input> in;
	in.push(gzip_decompressor());
	in.push(pdbFile);
	boost::iostreams::copy(in, cout);
    // TODO - parse pdb file
    return 0;
}
