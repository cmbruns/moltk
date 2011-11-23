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
 *  Alignment class representing a sequence alignment
 */

#ifndef MOLTK_ALIGN_ALIGNMENT_H
#define MOLTK_ALIGN_ALIGNMENT_H

#include "moltk/EString.hpp"
#include "moltk/PDBStructure.hpp"
#include "moltk/units.hpp"

namespace moltk {

/*!
 * Alignment represents a set of aligned macromolecule sequences and/or structures.
 */
template<class SCORE_TYPE>
class Alignment_
{
public:


    /// Whether a particular Alignment_ member is a sequence or structure.
    enum List {
        LIST_SEQUENCE, ///< Item belongs to the sequence list
        LIST_STRUCTURE ///< Item belongs to the structure list
    };

    typedef moltk::EString EString;

    /// Meta-data for one sequence in an Alignment_
    class Row
    {
    public:
        List list; ///< which list: sequences or structures?
        int list_index; ///< index into either the structure or sequence list
        Real sequence_weight; ///< relative contribution of this sequence to the alignment score
        EString e_string; ///< gap pattern of this sequence
    };


public:
    /// Default constructor creates an empty Alignment_
    Alignment_();
    /// Create an alignment with exactly one sequence
    /* implicit */ Alignment_(const Biosequence& sequence);
    /// Create an alignment from fasta sequences or a single sequence string
    /* implicit */ Alignment_(const std::string& alignment_string);
    /// Create an alignment from fasta sequences or a single sequence string
    /* implicit */ Alignment_(const char* alignment_string);
    /// Delete alignment (destructor)
    ~Alignment_() {}
    /// Add one sequence to the alignment.  Internally, gaps will be removed and encoded into an EString.
    Alignment_& append_sequence(const Biosequence& sequence);
    /// Add one sequence from a single sequence string.
    Alignment_& load_string(const std::string& alignment_string);
    /// Append sequences from a string containing fasta sequences
    Alignment_& load_fasta_string(const std::string& fasta_string);
    /// Load fasta format sequences from a C++ stream
    Alignment_& load_fasta(std::istream& input_stream);
    /// Load fasta sequences from named file
    Alignment_& load_fasta(const std::string& file_name);

    /// Write a pretty formatted alignment to a C++ stream
    void write_pretty(std::ostream& output_stream) const;
    /// Write a pretty formatted alignment to a file
    void write_pretty(const std::string& file_name) const;
    /// Create a string containing a pretty formatted alignment
    std::string pretty() const;

    /// Write Alignment_ in fasta format to a C++ stream
    void write_fasta(std::ostream& output_stream) const;
    /// Write Alignment_ in fasta format to a file
    void write_fasta(const std::string& file_name) const;
    /// Create a string with Alignment_ in fasta format
    std::string fasta() const;

    /// Write table of pairwise sequence identities to a C++ stream
    void write_id_table(std::ostream& output_stream) const;
    /// Write table of pairwise sequence identities to a file
    void write_id_table(const std::string& file_name) const;
    /// Create a string containing table of pairwise sequence identities
    std::string id_table() const;

    /// Number of columns (width) of sequence Alignment_
    size_t get_number_of_columns() const;
    /// get_number_of_sequences() includes combined number of both sequences and structures
    size_t get_number_of_sequences() const {return rows.size();}

    /// Align two sequence alignments using a pair of precomputed EStrings.
    ///
    /// This methods is used to create the final Alignment_ after the dynamic
    /// programming alignment has completed.
    Alignment_ align(const Alignment_&, const EString&, const EString&) const;

    /// Returns the particular sequence or structure at Row index
    const BaseBiosequence& get_sequence(size_t index) const;
    /// The gapping pattern of Row index
    const EString& get_estring(size_t index) const;
    /// The precomputed total sum of pairs score of this Alignment_
    const SCORE_TYPE& get_score() const;
    /// Set the sum of pairs score for this Alignment_.  Make sure you put the correct answer!
    Alignment_& set_score(const SCORE_TYPE& s);
    /// Access internal Alignment::Row data structure
    const Row& get_row(int index) const {return rows[index];}

    /// Create a shuffled version of this alignment to help estimate significance
    Alignment_ shuffle() const;

    /// Low level python string representation of this Alignment_
    std::string repr() const;
    Alignment_& set_pretty_width(int width) {pretty_width = width; return *this;}
    int get_pretty_width() const {return pretty_width;}
    inline friend std::ostream& operator<<(std::ostream& os, const moltk::Alignment_<SCORE_TYPE>& ali) {
        ali.write_pretty(os);
        return os;
    }

protected:
    std::vector<Biosequence> sequences;
    std::vector<PDBStructure::Chain> structures;
    std::vector<Row> rows;
    SCORE_TYPE m_score;
    int pretty_width;
};

typedef Alignment_<moltk::units::Information> Alignment;

/// global load_fasta method helps get SEQUOIA-like conciseness in python.
Alignment load_fasta(const std::string& file_name);

} // namespace moltk


#endif // MOLTK_ALIGN_ALIGNMENT_H
