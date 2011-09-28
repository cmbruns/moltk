#ifndef MOLTK_CHEM_ATOM_H
#define MOLTK_CHEM_ATOM_H

namespace moltk { namespace pdb {

/*!
 * Atom represents a chemical atom in a molecular structure.
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

}} // namespace moltk::pdb

#endif // MOLTK_CHEM_ATOM_H
