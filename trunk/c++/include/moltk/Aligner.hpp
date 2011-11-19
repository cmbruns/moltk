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

/*!
 *  \file
 *  Aligner class for aligning Biosequences and Alignments using a MatrixScorer.
 */

#ifndef MOLTK_ALIGN_ALIGNER_H
#define MOLTK_ALIGN_ALIGNER_H

#include "moltk/DPTable.hpp"

namespace moltk {

/*! The Aligner class creates macromolecule sequence alignments and structure alignments.
 *
 */
class Aligner 
{
public:

    // typedef moltk::units::Information Information;

public:
    /// Construct an Aligner with default parameters and no Alignments yet
    Aligner();
    Alignment align(const Alignment&, const Alignment&);
    /// scorer computes alignment scores.
    const MatrixScorer& get_scorer() const {return scorer;}
    /// MatrixScorer object this Aligner uses to score alignments.
    MatrixScorer& get_scorer() {return scorer;}
    Aligner& set_scorer(const MatrixScorer& scorer) {this->scorer = scorer; return *this;}

   /*!
     * Global shared aligner object used by align() method.
     */
    static Aligner& get_shared_aligner();

    /// The actual dynamic programming table
    dp::DPTable<moltk::units::Information, dp::DP_MEMORY_LARGE, dp::DP_ALIGN_GAPPED_ALIGNMENTS, 1> test_table;

protected:
    MatrixScorer scorer;
};

/*
 * global align() methods helps get SEQUOIA-like conciseness in python.
 */
Alignment align(const Alignment& target_alignment, const Alignment& query_alignment);

} // namespace moltk

#endif // MOLTK_ALIGN_ALIGNER_H

