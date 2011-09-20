#ifndef MOLTK_CHEM_PDB_STRUCTURE_H
#define MOLTK_CHEM_PDB_STRUCTURE_H

#include <iostream>
#include <vector>
#include <string>
#include "Atom.h"

namespace moltk { namespace pdb {
	
class Structure
{
public:
    Structure();
    explicit Structure(std::istream&);
    explicit Structure(const std::string& fileName);

    bool loadFromStream(std::istream&);
    bool loadFromFile(const std::string& fileName);

    std::vector<Atom> atoms;
};
	
}}

#endif // MOLTK_PDB_STRUCTURE_H
