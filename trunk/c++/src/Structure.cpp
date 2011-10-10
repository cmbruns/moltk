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
    
    For questions contact: cmbruns@rotatingpenguin.com
*/

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

