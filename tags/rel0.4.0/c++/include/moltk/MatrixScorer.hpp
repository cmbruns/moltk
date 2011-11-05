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

#ifndef MOLTK_ALIGN_MATRIX_SCORER_H
#define MOLTK_ALIGN_MATRIX_SCORER_H

#include "moltk/Aligner.hpp"
#include "moltk/units.hpp"
#include <iostream>
#include <vector>

namespace moltk {

/*!
 * MatrixScorer scores alignments using a residue type
 * matrix such as BLOSUM62 or PAM250.
 */
class MatrixScorer : public Aligner::Scorer
{
public:
    typedef moltk::units::Information Information;

    explicit MatrixScorer(const std::string& matrix_string);
    explicit MatrixScorer(std::istream& matrix_stream);
    std::vector<Aligner::QueryPosition*> create_query_positions(const Alignment& alignment) const;
    std::vector<Aligner::TargetPosition*> create_target_positions(const Alignment& alignment) const;
    std::istream& load_stream(std::istream&);

    static const MatrixScorer& get_blosum62_scorer();

    /*!
     * QueryPosition represents an alignment/sequence column in the
     * second of two sequences being scored by a MatrixScorer.
     *
    // Query alignment appears after target alignment in combined alignment.
    // Query alignment should be shorter than target alignment for small-memory optimizaion.
    // Query alignment should have fewer/less-diverse sequences for scoring optimization.
     */
    class QueryPosition : public Aligner::QueryPosition
    {
    public:

        virtual QueryPosition* clone() const;
        virtual Information get_gap_open_penalty() const {return gap_open_penalty;}
        virtual Information get_gap_extension_penalty() const {return gap_extension_penalty;}
        inline friend std::ostream& operator<<(std::ostream& os, const QueryPosition& p)
        {
            os << "op=" << p.get_gap_open_penalty();
            os << ", ep=" << p.get_gap_extension_penalty();
            return os;
        }

        Information gap_open_penalty;
        Information gap_extension_penalty;
        // cache values for quick score lookup
        typedef std::vector< std::pair<size_t, double> > QueryWeights;
        QueryWeights residue_type_index_weights;
    };


    /*!
     * TargetPosition represents an alignment/sequence column in the
     * first of two sequences being scored by a MatrixScorer.
     */
    class TargetPosition : public Aligner::TargetPosition
    {
    public:

        virtual TargetPosition* clone() const;
        virtual moltk::units::Information score(const Aligner::QueryPosition& rhs) const;
        virtual Information get_gap_open_penalty() const {return gap_open_penalty;}
        virtual Information get_gap_extension_penalty() const {return gap_extension_penalty;}

        Information gap_open_penalty;
        Information gap_extension_penalty;
        // cache values for quick score lookup
        std::vector<Information> scores_by_residue_type_index;
    };

protected:
    // create_foo_positions is a helper function to reduce redundancy between 
    // methods create_query_positions() and create_target_positions()
    template<class POSB, class POS>
    std::vector<POSB*> create_foo_positions(const Alignment& alignment) const;

    std::vector<int> character_indices; // maps letters to matrix indices
    std::vector< std::vector<Information> > matrix;
};

std::istream& operator>>(std::istream&, moltk::MatrixScorer&);

} // namespace moltk

#endif // MOLTK_ALIGN_MATRIX_SCORER_H

