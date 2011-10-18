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

    // Query alignment appears after target alignment in combined alignment.
    // Query alignment should be shorter than target alignment for small-memory optimizaion.
    // Query alignment should have fewer/less-diverse sequences for scoring optimization.
    class QueryPosition : public Aligner::QueryPosition
    {
    public:

        virtual QueryPosition* clone() const;
        virtual Information get_gap_open_penalty() const {return m_gapOpenPenalty;}
        virtual Information get_gap_extension_penalty() const {return m_gapExtensionPenalty;}
        inline friend std::ostream& operator<<(std::ostream& os, const QueryPosition& p)
        {
            os << "op=" << p.get_gap_open_penalty();
            os << ", ep=" << p.get_gap_extension_penalty();
            return os;
        }

        Information m_gapOpenPenalty;
        Information m_gapExtensionPenalty;
        // cache values for quick score lookup
        typedef std::vector< std::pair<size_t, double> > QueryWeights;
        QueryWeights residueTypeIndexWeights;
    };


    class TargetPosition : public Aligner::TargetPosition
    {
    public:

        virtual TargetPosition* clone() const;
        virtual moltk::units::Information score(const Aligner::QueryPosition& rhs) const;
        virtual Information get_gap_open_penalty() const {return m_gapOpenPenalty;}
        virtual Information get_gap_extension_penalty() const {return m_gapExtensionPenalty;}

        Information m_gapOpenPenalty;
        Information m_gapExtensionPenalty;
        // cache values for quick score lookup
        std::vector<Information> scoresByResidueTypeIndex;
    };

protected:
    // createFooPositions is a helper function to reduce redundancy between 
    // methods createQueryPositions() and createTargetPositions()
    template<class POSB, class POS>
    std::vector<POSB*> createFooPositions(const Alignment& alignment) const;

    std::vector<int> characterIndices; // maps letters to matrix indices
    std::vector< std::vector<Information> > matrix;
};

std::istream& operator>>(std::istream&, moltk::MatrixScorer&);

} // namespace moltk

#endif // MOLTK_ALIGN_MATRIX_SCORER_H

