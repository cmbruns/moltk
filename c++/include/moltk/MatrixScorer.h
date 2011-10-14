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
    std::vector<Aligner::QueryPosition*> createQueryPositions(const Alignment&) const;
    std::vector<Aligner::TargetPosition*> createTargetPositions(const Alignment&) const;
    std::istream& loadStream(std::istream&);

    static const MatrixScorer& getBlosum62Scorer();

    class QueryPosition : public Aligner::QueryPosition
    {
    public:

        virtual QueryPosition* clone() const;
        virtual Information gapOpenPenalty() const {return m_gapOpenPenalty;}
        virtual Information gapExtensionPenalty() const {return m_gapExtensionPenalty;}

        Information m_gapOpenPenalty;
        Information m_gapExtensionPenalty;
        // cache values for quick score lookup
        double scoreWeight;
        int columnIndex;
    };


    class TargetPosition : public Aligner::TargetPosition
    {
    public:

        virtual TargetPosition* clone() const;
        virtual moltk::units::Information score(const Aligner::QueryPosition& rhs) const;
        virtual Information gapOpenPenalty() const {return m_gapOpenPenalty;}
        virtual Information gapExtensionPenalty() const {return m_gapExtensionPenalty;}

        Information m_gapOpenPenalty;
        Information m_gapExtensionPenalty;
        // cache values for quick score lookup
        double scoreWeight;
        const Information* rowPtr;
    };

protected:
    std::vector<int> characterIndices; // maps letters to matrix indices
    std::vector< std::vector<Information> > matrix;
};

std::istream& operator>>(std::istream&, moltk::MatrixScorer&);

} // namespace moltk

#endif // MOLTK_ALIGN_MATRIX_SCORER_H

