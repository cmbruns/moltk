#ifndef MOLTK_CHEM_PDB_STRUCTURE_H
#define MOLTK_CHEM_PDB_STRUCTURE_H

#include <iostream>
#include <vector>
#include "Atom.h"

namespace moltk { namespace pdb {
	
class Structure
{
public:
    Structure();
    explicit Structure(std::istream&);
    explicit Structure(const char* fileName);

    void loadFromStream(std::istream&);
    void loadFromFile(const char* fileName);

    std::vector<Atom> atoms;
};
	
}}

#endif // MOLTK_PDB_STRUCTURE_H
