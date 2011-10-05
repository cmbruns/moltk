#ifndef MOLTK_CHEM_PDB_STRUCTURE_H
#define MOLTK_CHEM_PDB_STRUCTURE_H

#include <iostream>
#include <vector>
#include <string>
#include "moltk/Biosequence.h"

namespace moltk {
	
class Structure : public BaseBiosequence
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


    class Residue : public BaseResidue
    {
    public:
        virtual char getOneLetterCode() const {return oneLetterCode;}
        virtual int getResidueNumber() const {return residueNumber;}

    protected:
        char oneLetterCode;
        int residueNumber;
    };

public:
    Structure() {}
    explicit Structure(std::istream&);
    explicit Structure(const std::string& fileName);

    bool loadStream(std::istream&);
    bool loadFile(const std::string& fileName);
    virtual size_t getNumberOfResidues() const {return residues.size();}
    virtual const BaseResidue& getResidue(size_t index) const {
        return residues[index];
    }

protected:
    AtomList atoms;
    std::vector<Residue> residues;
};

} // namespace moltk

inline std::istream& operator>>(std::istream& is, moltk::Structure& structure)
{
    structure.loadStream(is);
    return is;
}


#endif // MOLTK_PDB_STRUCTURE_H
