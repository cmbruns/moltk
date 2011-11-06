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

#ifndef MOLTK_ALIGN_ALIGNMENT_H
#define MOLTK_ALIGN_ALIGNMENT_H

#include "moltk/EString.hpp"
#include "moltk/Biosequence.hpp"
#include "moltk/PDBStructure.hpp"
#include "moltk/Real.hpp"
#include "moltk/units.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace moltk {

/*!
 * Alignment represents a set of aligned macromolecule sequences and/or structures.
 */
class Alignment
{
public:


    /// Whether a particular Alignment member is a sequence or structure.
    enum List {
        LIST_SEQUENCE, ///< Item belongs to the sequence list
        LIST_STRUCTURE ///< Item belongs to the structure list
    };

    typedef moltk::EString EString;

    /// Meta-data for one sequence in an Alignment
    class Row
    {
    public:
        List list; ///< which list: sequences or structures?
        int list_index; ///< index into either the structure or sequence list
        Real sequence_weight; ///< relative contribution of this sequence to the alignment score
        EString e_string; ///< gap pattern of this sequence
    };


public:
    /// Default constructor creates an empty Alignment
    Alignment();
    /// Create an alignment with exactly one sequence
    /* implicit */ Alignment(const Biosequence& sequence);
    /// Create an alignment from fasta sequences or a single sequence string
    /* implicit */ Alignment(const std::string& alignment_string);
    /// Create an alignment from fasta sequences or a single sequence string
    /* implicit */ Alignment(const char* alignment_string);
    /// Delete alignment (destructor)
    ~Alignment() {}
    /// Add one sequence to the alignment.  Internally, gaps will be removed and encoded into an EString.
    Alignment& append_sequence(const Biosequence& sequence);
    /// Add sequences from fasta sequences or a single sequence string.
    void load_string(const std::string& alignment_string);
    /// Load fasta format sequences from a C++ stream
    Alignment& load_fasta(std::istream& input_stream);
    /// Load fasta sequences from named file
    Alignment& load_fasta(const std::string& file_name);

    /// Write a pretty formatted alignment to a C++ stream
    void write_pretty(std::ostream& output_stream) const;
    /// Write a pretty formatted alignment to a file
    void write_pretty(const std::string& file_name) const;
    /// Create a string containing a pretty formatted alignment
    std::string pretty() const;

    /// Write Alignment in fasta format to a C++ stream
    void write_fasta(std::ostream& output_stream) const;
    /// Write Alignment in fasta format to a file
    void write_fasta(const std::string& file_name) const;
    /// Create a string with Alignment in fasta format
    std::string fasta() const;

    /// Write table of pairwise sequence identities to a C++ stream
    void write_id_table(std::ostream& output_stream) const;
    /// Write table of pairwise sequence identities to a file
    void write_id_table(const std::string& file_name) const;
    /// Create a string containing table of pairwise sequence identities
    std::string id_table() const;

    /// Number of columns (width) of sequence Alignment
    size_t get_number_of_columns() const;
    /// get_number_of_sequences() includes combined number of both sequences and structures
    size_t get_number_of_sequences() const {return rows.size();}

    /// Align two sequence alignments using a pair of precomputed EStrings.
    ///
    /// This methods is used to create the final Alignment after the dynamic
    /// programming alignment has completed.
    Alignment align(const Alignment&, const EString&, const EString&) const;

    /// Returns the particular sequence or structure at Row index
    const BaseBiosequence& get_sequence(size_t index) const;
    /// The gapping pattern of Row index
    const EString& get_estring(size_t index) const;
    /// The precomputed total sum of pairs score of this Alignment
    const moltk::units::Information& get_score() const;
    /// Set the sum of pairs score for this Alignment.  Make sure you put the correct answer!
    Alignment& set_score(const moltk::units::Information& s);
    /// Inefficient computation of sum-of-pairs score, for use in testing and debugging.
    units::Information calc_explicit_sum_of_pairs_score() const;
    /// Compute pair score between two sequences in this alignment
    units::Information calc_explicit_pair_score(int i, int j) const;
    /// Low level python string representation of this Alignment
    std::string repr() const;
    Alignment& set_pretty_width(int width) {pretty_width = width; return *this;}
    int get_pretty_width() const {return pretty_width;}

protected:
    std::vector<Biosequence> sequences;
    std::vector<PDBStructure::Chain> structures;
    std::vector<Row> rows;
    moltk::units::Information m_score;
    int pretty_width;
};


/// global load_fasta method helps get SEQUOIA-like conciseness in python.
Alignment load_fasta(const std::string& file_name);

/// Standard print method for Alignment produces pretty formatted output.
std::ostream& operator<<(std::ostream& os, const Alignment& ali);

} // namespace moltk


#endif // MOLTK_ALIGN_ALIGNMENT_H
