#ifndef MOLTK_CHEM_PDB_STRUCTURE_H
#define MOLTK_CHEM_PDB_STRUCTURE_H

#include <iostream>
#include <vector>
#include <string>

namespace moltk {
	
class Structure
{
public:
    /*!
     * Structure::Atom represents a chemical atom in a molecular structure.
     */
    class Atom
    {
    public:
        enum RecordType {
            ATOM_RECORD_TYPE, //< Standard protein, DNA, or RNA atoms
            HETATM_RECORD_TYPE //< Other atom types
        };
    
        /// Test function to test python bindings.
        int foo() const {return 5;}
    
        /// Whether this is a standard macromolecule atom type (ATOM) or other (HETATM).
        RecordType getRecordType() const {return recordType;}
    
    protected:
        RecordType recordType;
    };
    typedef std::vector<Atom> AtomList;


public:
    Structure() {}
    explicit Structure(std::istream&);
    explicit Structure(const std::string& fileName);

    bool loadStream(std::istream&);
    bool loadFile(const std::string& fileName);

protected:
    AtomList atoms;
};

} // namespace moltk

static std::istream& operator>>(std::istream& is, moltk::Structure& structure)
{
    structure.loadStream(is);
    return is;
}


#endif // MOLTK_PDB_STRUCTURE_H
