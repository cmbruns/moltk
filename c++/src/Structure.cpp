#include "moltk/Structure.h"
#include <string>
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

namespace moltk {

/* explicit */
Structure::Structure(const std::string& fileName)
{
    loadFile(fileName);
}

/* explicit */
Structure::Structure(std::istream& istream)
{
    loadStream(istream);
}

bool Structure::loadFile(const std::string& fileName)
{
    ifstream pdbFile(fileName.c_str(), ios_base::in | ios_base::binary);
    if (! pdbFile.good()) {
        cerr << "Problem opening file " << fileName << endl;
        return false;
    }
	filtering_streambuf<input> in;
    if ( (fileName.substr(fileName.length() - 3) == ".gz") 
      || (fileName.substr(fileName.length() - 3) == ".GZ") )
    {
    	in.push(gzip_decompressor());
    }
	in.push(pdbFile);
    std::istream inStream(&in);
    return loadStream(inStream);
}

/* explicit */
bool Structure::loadStream(std::istream& istream)
{
    if (!istream.good())
        return false;
    std::string currentLine;
    int atomCount = 0;
    while (istream.good()) 
    {
        getline(istream, currentLine);
        if ( (currentLine.substr(0, 6) == "ATOM  ") 
          || (currentLine.substr(0, 6) == "HETATM") )
        {
            ++atomCount;
        }
    }
    cerr << atomCount << " atoms found." << endl;
    return true;
}

} // namespace moltk

