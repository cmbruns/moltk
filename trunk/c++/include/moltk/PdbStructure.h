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

#ifndef MOLTK_CHEM_PDB_STRUCTURE_H
#define MOLTK_CHEM_PDB_STRUCTURE_H

#include <iostream>
#include <vector>
#include <string>
#include "moltk/Biosequence.h"
#include "moltk/Printable.h"

namespace moltk {
	
class PdbStructure
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
        /// Whether this is a standard macromolecule atom type (ATOM) or other (HETATM).
        RecordType getRecordType() const {return recordType;}
    
    protected:
        RecordType recordType;
    };
    typedef std::vector<Atom> AtomList;


    class Residue : public BaseBiosequence::BaseResidue
    {
    public:
        Residue() {}
        Residue(const Residue& rhs)
            : one_letter_code(rhs.one_letter_code)
            , residue_number(rhs.residue_number)
        {}
        virtual char get_one_letter_code() const {return one_letter_code;}
        virtual int get_residue_number() const {return residue_number;}

    protected:
        char one_letter_code;
        int residue_number;
    };

    class Chain : public BaseBiosequence
    {
    public:
        Chain() {}
        Chain(const Chain& rhs) : residues(rhs.residues) {}
        virtual size_t get_number_of_residues() const {return residues.size();}
        virtual const BaseResidue& get_residue(size_t index) const {
            return residues[index];
        }
    protected:
        std::vector<Residue> residues;
    };

public:
    PdbStructure() {}
    explicit PdbStructure(std::istream&);
    explicit PdbStructure(const std::string& fileName);
    PdbStructure(const PdbStructure& rhs)
        : atoms(rhs.atoms)
        , chains(rhs.chains)
    {}
    bool loadStream(std::istream&);
    bool loadFile(const std::string& fileName);

protected:
    AtomList atoms;
    std::vector<Chain> chains;
};

} // namespace moltk

inline std::istream& operator>>(std::istream& is, moltk::PdbStructure& structure)
{
    structure.loadStream(is);
    return is;
}


#endif // MOLTK_PDB_STRUCTURE_H
