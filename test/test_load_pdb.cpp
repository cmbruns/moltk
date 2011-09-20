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
using namespace moltk;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "File name argument missing" << endl;
        return 1; // file name argument missing
    }
    const char* pdbFileName = argv[1];
    ifstream pdbFile(pdbFileName, ios_base::in | ios_base::binary);
    if (! pdbFile.good()) {
        cerr << "Problem opening file " << pdbFileName << endl;
        return 1;
    }
	filtering_streambuf<input> in;
	in.push(gzip_decompressor());
	in.push(pdbFile);
    std::istream inStream(&in);

    cerr << "Printing file contents..." << endl;
	// boost::iostreams::copy(in, cout);

    pdb::Structure pdbStructure(inStream);
    // TODO - parse pdb file

    return 0;
}
