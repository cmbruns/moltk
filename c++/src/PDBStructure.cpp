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

#include "moltk/PDBStructure.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

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
PDBStructure::PDBStructure(const std::string& fileName)
{
    load_file(fileName);
}

/* explicit */
PDBStructure::PDBStructure(std::istream& istream)
{
    load_stream(istream);
}

bool PDBStructure::load_file(const std::string& fileName)
{
    ifstream pdb_file;
    pdb_file.open(fileName.c_str(), ios_base::in | ios_base::binary);
    if (!pdb_file)
        throw std::runtime_error("Error opening PDB file");
	filtering_streambuf<input> in;
    if ( (fileName.substr(fileName.length() - 3) == ".gz") 
      || (fileName.substr(fileName.length() - 3) == ".GZ") )
    {
    	in.push(gzip_decompressor());
    }
	in.push(pdb_file);
    std::istream inStream(&in);
    return load_stream(inStream);
}

/* explicit */
bool PDBStructure::load_stream(std::istream& istream)
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

