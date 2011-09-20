#include "chem/Structure.h"
#include <string>
#include <iostream>

using namespace std;

namespace moltk { namespace pdb {

Structure::Structure(std::istream& istream)
{
    loadFromStream(istream);
}

/* explicit */
void Structure::loadFromStream(std::istream& istream)
{
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
}

}} // namespace moltk::pdb
