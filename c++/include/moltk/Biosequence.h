/*
 * Biosequence.h
 *
 *  Created on: Oct 5, 2011
 *      Author: brunsc
 */

#ifndef BIOSEQUENCE_H_
#define BIOSEQUENCE_H_

#include "moltk/Printable.h"
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


class Biosequence : public BaseBiosequence
{
public:


    class Residue : public BaseResidue
    {
    public:
        Residue(char oneLetterCodeParam, int residueNumberParam)
            : oneLetterCode(oneLetterCodeParam)
            , residueNumber(residueNumberParam)
        {}
        Residue(const Residue& rhs)
            : oneLetterCode(rhs.oneLetterCode)
            , residueNumber(rhs.residueNumber)
        {}
        virtual char getOneLetterCode() const {return oneLetterCode;}
        virtual int getResidueNumber() const {return residueNumber;}

    protected:
        char oneLetterCode;
        int residueNumber;
    };


    // STL-conformant iterators
    typedef std::vector<Residue> ResidueList;
    typedef ResidueList::iterator iterator;
    typedef ResidueList::const_iterator const_iterator;

public:
    Biosequence() {}
    /* implicit */ Biosequence(const std::string& str);
    Biosequence(const Biosequence& rhs)
        : residues(rhs.residues)
    {}
    virtual ~Biosequence() {}
    virtual size_t getNumberOfResidues() const {return residues.size();}
    virtual const BaseResidue& getResidue(size_t index) const {
        return residues[index];
    }
    iterator begin() {return residues.begin();}
    const_iterator begin() const {return residues.begin();}
    iterator end() {return residues.end();}
    const_iterator end() const {return residues.end();}

protected:
    ResidueList residues;
};

} // namespace moltk

#endif /* BIOSEQUENCE_H_ */
