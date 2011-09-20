#ifndef MOLTK_CHEM_PDB_STRUCTURE_H
#define MOLTK_CHEM_PDB_STRUCTURE_H

#include <iostream>
#include <vector>
#include "Atom.h"

namespace moltk { namespace pdb {
	
class Structure
{
public:
    Structure(std::istream&);

    std::vector<Atom> atoms;
};
	
}}

#endif // MOLTK_PDB_STRUCTURE_H
