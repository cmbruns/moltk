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

#ifndef MOLTK_ALIGN_MATRIX_SCORER_H
#define MOLTK_ALIGN_MATRIX_SCORER_H

#include "moltk/Aligner.h"
#include "moltk/units.h"
#include <iostream>
#include <vector>

namespace moltk {

// MatrixScorer knows how to lookup residue alignment scores from 
// a matrix such as BLOSUM62
class MatrixScorer : public Aligner::Scorer
{
public:
    typedef moltk::units::Information Information;

    explicit MatrixScorer(const std::string& matrixString);
    explicit MatrixScorer(std::istream& matrixStream);
    virtual Aligner::Position* createPosition(char sequenceLetter, bool bTerminus = false) const;
    std::istream& loadStream(std::istream&);

    static const MatrixScorer& getBlosum62Scorer();


    class Position : public Aligner::Position
    {
    public:
        virtual Position* clone() const;
        virtual moltk::units::Information score(const Aligner::Position& rhs) const;
        virtual char getOneLetterCode() const {return oneLetterCode;}
        virtual Information gapOpenPenalty() const {return m_gapOpenPenalty;}
        virtual Information gapExtensionPenalty() const {return m_gapExtensionPenalty;}

        Information m_gapOpenPenalty;
        Information m_gapExtensionPenalty;
        // cache values for quick score lookup
        double scoreWeight;
        int columnIndex;
        const Information* rowPtr;
        char oneLetterCode;
    };

protected:
    std::vector<int> characterIndices; // maps letters to matrix indices
    std::vector< std::vector<Information> > matrix;
};

} // namespace moltk

std::istream& operator>>(std::istream&, moltk::MatrixScorer&);

#endif // MOLTK_ALIGN_MATRIX_SCORER_H

