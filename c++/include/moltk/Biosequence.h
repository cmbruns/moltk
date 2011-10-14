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

/*
 * Biosequence.h
 *
 *  Created on: Oct 5, 2011
 *      Author: brunsc
 */

#ifndef BIOSEQUENCE_H_
#define BIOSEQUENCE_H_

#include <vector>
#include <string>
#include <iterator>

namespace moltk
{

class BaseBiosequence
{
public:

    class BaseResidue
    {
    public:
        BaseResidue() {}
        virtual char getOneLetterCode() const = 0;
        virtual int getResidueNumber() const = 0;
        operator char() const {return getOneLetterCode();}

    private:
        BaseResidue(const BaseResidue&);
    };


    // TODO - Does GapResidue belong somewhere else?  Biosequence or Alignment?
    /// GapResidue is used in Alignments
    class GapResidue : public BaseResidue
    {
    public:
        /// GapResidue has the residue number of the preceding non-gap residue.
        ///
        /// Thus a left end-gap might have a residue number of zero.
        explicit GapResidue(int residueNumberParam)
            : residueNumber(residueNumberParam)
        {}
        virtual char getOneLetterCode() const {return '-';}
        virtual int getResidueNumber() const {return residueNumber;};

    protected:
        int residueNumber;
    };


public:
    BaseBiosequence() {}
    virtual ~BaseBiosequence() {}
    virtual void print_to_stream(std::ostream& os) const;
    virtual size_t getNumberOfResidues() const = 0;
    virtual const BaseResidue& getResidue(size_t index) const = 0;

private:
    BaseBiosequence(const BaseBiosequence&);
};


class BiosequenceResidue : public BaseBiosequence::BaseResidue
{
public:
    BiosequenceResidue(char oneLetterCodeParam, int residueNumberParam)
        : oneLetterCode(oneLetterCodeParam)
        , residueNumber(residueNumberParam)
    {}
    BiosequenceResidue(const BiosequenceResidue& rhs)
        : oneLetterCode(rhs.oneLetterCode)
        , residueNumber(rhs.residueNumber)
    {}
    virtual char getOneLetterCode() const {return oneLetterCode;}
    virtual int getResidueNumber() const {return residueNumber;}

protected:
    char oneLetterCode;
    int residueNumber;
};


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
    void loadStream(std::istream& is);
    size_t getNumberOfResidues() const { return size(); }
    const Residue& getResidue(size_t ix) const { return (*this)[ix]; }
    void printString(std::ostream& os) const
    {
        for (const_iterator i = begin(); i != end(); ++i)
            os << *i;
    }

    inline friend std::ostream& operator<<(std::ostream& os, const Biosequence& seq)
    {
        seq.printString(os);
        return os;
    }

    inline friend std::istream& operator>>(std::istream& is, Biosequence& seq)
    {
        seq.loadStream(is);
        return is;
    }

protected:
    std::string description;
};


} // namespace moltk

#endif /* BIOSEQUENCE_H_ */
