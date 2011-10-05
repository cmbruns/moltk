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

namespace moltk
{

class BaseBiosequence
{
public:

    class BaseResidue
    {
    public:
        virtual char getOneLetterCode() const = 0;
        virtual int getResidueNumber() const = 0;
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
    virtual ~BaseBiosequence() {}
    virtual void print_to_stream(std::ostream& os) const;
    virtual size_t getNumberOfResidues() const = 0;
    virtual const BaseResidue& getResidue(size_t index) const = 0;
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
        virtual char getOneLetterCode() const {return oneLetterCode;}
        virtual int getResidueNumber() const {return residueNumber;}

    protected:
        char oneLetterCode;
        int residueNumber;
    };


public:
    /* implicit */ Biosequence(const std::string& str);
    virtual ~Biosequence() {}
    virtual size_t getNumberOfResidues() const {return residues.size();}
    virtual const BaseResidue& getResidue(size_t index) const {
        return residues[index];
    }

protected:
    std::vector<Residue> residues;
};

} // namespace moltk

#endif /* BIOSEQUENCE_H_ */
