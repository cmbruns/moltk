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

/*
 * Biosequence.h
 *
 *  Created on: Oct 5, 2011
 *      Author: brunsc
 */

#ifndef BIOSEQUENCE_H_
#define BIOSEQUENCE_H_

#include <sstream>
#include <vector>
#include <string>
#include <iterator>

namespace moltk
{

/*!
 * Parent class for macromolecule sequences and structures.
 */
class BaseBiosequence
{
public:

    /*!
     * Parent class for macromolecule sequence residues (nucleotides or amino acids)
     */
    class BaseResidue
    {
    public:
        BaseResidue() {}
        virtual char get_one_letter_code() const = 0;
        virtual int get_residue_number() const = 0;
        operator char() const {return get_one_letter_code();}

    private:
        BaseResidue(const BaseResidue&);
    };


public:
    BaseBiosequence() {}
    virtual ~BaseBiosequence() {}
    virtual void print_to_stream(std::ostream& os) const;
    virtual size_t get_number_of_residues() const = 0;
    virtual const BaseResidue& get_residue(size_t index) const = 0;

private:
    BaseBiosequence(const BaseBiosequence&);
};


/*!
 * One residue in a macromolecule sequence (a nucleotide or amino acid)
 */
class BiosequenceResidue : public BaseBiosequence::BaseResidue
{
public:
    BiosequenceResidue(char one_letter_code_param, int residue_number_param)
        : one_letter_code(one_letter_code_param)
        , residue_number(residue_number_param)
    {}
    BiosequenceResidue(const BiosequenceResidue& rhs)
        : one_letter_code(rhs.one_letter_code)
        , residue_number(rhs.residue_number)
    {}
    virtual char get_one_letter_code() const {return one_letter_code;}
    virtual int get_residue_number() const {return residue_number;}

protected:
    char one_letter_code;
    int residue_number;
};


/*!
 * A macromolecule sequence (DNA or RNA or protein)
 */
class Biosequence : public BaseBiosequence, public std::vector<BiosequenceResidue>
{
public:
    typedef BiosequenceResidue Residue;

public:
    Biosequence() {}
    Biosequence(const std::string& sequence, const std::string& description = "");
    Biosequence(const char * sequence, const std::string& description = "");
    Biosequence(const Biosequence& rhs);
    virtual ~Biosequence() {}
    void load_stream(std::istream& is);
    Biosequence& load_fasta(const std::string& file_name);
    Biosequence& load_fasta(std::istream& is);
    size_t get_number_of_residues() const { return size(); }
    const Residue& get_residue(size_t ix) const { return (*this)[ix]; }
    void print_string(std::ostream& os) const;
    /*!
     * repr is a helper for the python __repr__ method.
     */
    std::string repr() const;

    inline friend std::ostream& operator<<(std::ostream& os, const Biosequence& seq)
    {
        seq.print_string(os);
        return os;
    }

    inline friend std::istream& operator>>(std::istream& is, Biosequence& seq)
    {
        seq.load_stream(is);
        return is;
    }

protected:
    std::string description;
};

Biosequence load_fasta(const std::string& file_name);

} // namespace moltk

#endif /* BIOSEQUENCE_H_ */
