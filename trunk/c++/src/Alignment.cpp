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

#include "moltk/Alignment.hpp"
#include <cassert>
#include <cctype> // toupper
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>

using namespace moltk;
using namespace std;


///////////////////////
// Alignment methods //
///////////////////////

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>::Alignment_() 
    : m_score(moltk::units::zero<SCORE_TYPE>()) 
    , pretty_width(50)
{}

/* implicit */
template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>::Alignment_(const Biosequence& seq) 
 : m_score(moltk::units::zero<SCORE_TYPE>())
 , pretty_width(50)
{
    append_sequence(seq);
}

/* implicit */ 
template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>::Alignment_(const std::string& s)
 : m_score(moltk::units::zero<SCORE_TYPE>())
 , pretty_width(50)
{
    load_string(s);
}

/* implicit */ 
template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>::Alignment_(const char* str)
 : m_score(moltk::units::zero<SCORE_TYPE>())
 , pretty_width(50)
{
    std::string s(str);
    load_string(s);
}

template<class SCORE_TYPE>
size_t Alignment_<SCORE_TYPE>::get_number_of_columns() const
{
    if (rows.size() == 0) return 0;
    return rows[0].e_string.total_length();
}

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>& Alignment_<SCORE_TYPE>::load_string(const std::string& s)
{
    Biosequence b(s);
    append_sequence(b);
    return *this;
}

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>& Alignment_<SCORE_TYPE>::append_sequence(const Biosequence& seq)
{
    rows.push_back(Row(seq));
    return *this;
}

template<class SCORE_TYPE>
void Alignment_<SCORE_TYPE>::write_fasta(std::ostream& output_stream) const
{
    for (size_t rowIx = 0; rowIx < rows.size(); ++rowIx) 
    {
        const Row& row = rows[rowIx];
        const BaseBiosequence* seq = &row.get_sequence();
        output_stream << '>' << row.get_description() << endl;
        EString::const_iterator i = row.e_string.begin();
        while(i != row.e_string.end()) {
            int resIx = *i;
            if (resIx < 0)
                output_stream << '-'; // gap
            else
                output_stream << seq->get_residue(resIx).get_one_letter_code();
            ++i;
        }
        output_stream << endl;
    }
}

template<class SCORE_TYPE>
void Alignment_<SCORE_TYPE>::write_fasta(const std::string& file_name) const
{
    ofstream output_stream;
    output_stream.open(file_name.c_str());
    if (!output_stream) {
        std::string msg("Error: moltk.Alignment unable to write to fasta file ");
        msg += file_name;
        throw std::runtime_error(msg.c_str());
    }
    write_fasta(output_stream);
}

template<class SCORE_TYPE>
std::string Alignment_<SCORE_TYPE>::fasta() const
{
    ostringstream output_stream;
    write_fasta(output_stream);
    return output_stream.str();
}

template<class SCORE_TYPE>
void Alignment_<SCORE_TYPE>::write_pretty(std::ostream& os) const
{
    size_t n_cols = get_number_of_columns();
    size_t n_seqs = get_number_of_sequences();
    std::vector<EString::const_iterator> iseq(n_seqs); // iterators into gapped sequences
    std::vector<int> last_res(n_seqs, 1); // most recently seen residue number
    std::vector<char> identities(n_cols, '*'); // start optimistic

    // print sequence descriptions in first block
    for (size_t rowIx = 0; rowIx < rows.size(); ++rowIx) 
    {
        // sequence number
        if (n_seqs > 1) {
            os.width(3);
            os << rowIx + 1 << ") ";
        } else {
            os << ">";
        }
        const Row& row = rows[rowIx];
        iseq[rowIx] = row.e_string.begin(); // initialize iterators
        os << row.get_description();
        os << endl;
    }

    // loop over blocks of columns
    size_t start_col = 0;
    while(start_col < n_cols)
    {
        size_t end_col = start_col + pretty_width - 1;
        if (end_col >= n_cols)
            end_col = n_cols - 1;

        if (n_seqs > 1) {
            os << endl; // blank line between blocks
            // one line with dots every ten positions
            os << "     ";
            for(size_t col = start_col; col <= end_col; ++col)
            {
                if (! ((col + 1) % 10))
                    os << '.';
                else
                    os << ' ';
            }
            os.width(5);
            os << end_col + 1;
            os << endl;
        }

        // one line for each sequence
        for (size_t s = 0; s < n_seqs; ++s)
        {
            // sequence number
            if (n_seqs > 1) {
                os.width(3);
                os << s + 1 << ") ";
            }
            const Row& row = rows[s];
            const BaseBiosequence* seq;
            seq = &row.get_sequence();
            // sequence characters
            for(size_t col = start_col; col <= end_col; ++col)
            {
                int resIx = *iseq[s];
                if (resIx < 0) {
                    os << '-'; // gap
                    identities[col] = ' '; // not identical
                }
                else {
                    char olc = seq->get_residue(resIx).get_one_letter_code();
                    os << olc;
                    last_res[s] = seq->get_residue(resIx).get_residue_number();
                    char& iden = identities[col];
                    if (iden == '*')
                        iden = olc;
                    else if (iden == olc)
                        ; // still identical
                    else
                        iden = ' '; // not identical
                }
                ++iseq[s];
            }
            os.width(5);
            os << last_res[s]; // final sequence number
            os << endl;
        }

        // one line with stars for identities
        if (n_seqs > 1) {
            os << "     ";
            for(size_t col = start_col; col <= end_col; ++col)
            {
                if (identities[col] == ' ')
                    os << ' ';
                else
                    os << '*';
            }
            os << endl;
        }

        start_col = end_col + 1;
    }
}

template<class SCORE_TYPE>
void Alignment_<SCORE_TYPE>::write_pretty(const std::string& file_name) const
{
    ofstream output_stream;
    output_stream.open(file_name.c_str());
    if (!output_stream) {
        std::string msg("Error: unable to write to pretty file ");
        msg += file_name;
        throw std::runtime_error(msg.c_str());
    }
    write_pretty(output_stream);
}
    
template<class SCORE_TYPE>
std::string Alignment_<SCORE_TYPE>::pretty() const
{
    ostringstream output_stream;
    write_pretty(output_stream);
    return output_stream.str();
}

template<class SCORE_TYPE>
void Alignment_<SCORE_TYPE>::write_id_table(std::ostream& output_stream) const
{
    size_t n_seq = get_number_of_sequences();

    // total_table contains number of residues aligned
    std::vector< std::vector<int> > total_table;
    total_table.assign(n_seq, std::vector<int>(n_seq, 0));

    // match_table contains number of same residues aligned.
    std::vector< std::vector<int> > match_table;
    match_table.assign(n_seq, std::vector<int>(n_seq, 0));

    std::vector<EString::const_iterator> iseq(n_seq); // iterators into gapped sequences
    for (size_t s = 0; s < n_seq; ++s) {
        const Row& row = rows[s];
        iseq[s] = row.e_string.begin(); // initialize iterators
    }
    size_t n_col = get_number_of_columns();
    for (size_t c = 0; c < n_col; ++c) {
        for (size_t s1 = 0; s1 < n_seq; ++s1) {
            int resIx1 = *iseq[s1];
            if (resIx1 < 0) continue; // gap
            const BaseBiosequence& seq1 = get_sequence(s1);
            char olc1 = std::toupper(seq1.get_residue(resIx1).get_one_letter_code());
            for (size_t s2 = 0; s2 < n_seq; ++s2) {
                int resIx2 = *iseq[s2];
                if (resIx2 < 0) continue; // gap
                total_table[s1][s2] += 1;
                total_table[s2][s1] += 1;
                // "so what" if we double count self residues; it cancels out.
                const BaseBiosequence& seq2 = get_sequence(s2);
                char olc2 = std::toupper(seq2.get_residue(resIx2).get_one_letter_code());
                if (olc1 == olc2) {
                    match_table[s1][s2] += 1;
                    match_table[s2][s1] += 1;
                }
            }
        }
        // advance iterators
        for (size_t s = 0; s < n_seq; ++s) {
            ++iseq[s];
        }
    }

    output_stream << "Percent sequence identities:" << endl;
    // header row with sequence numbers
    output_stream << "     ";
    for (size_t s = 0; s < n_seq; ++s) {
        output_stream.width(4);
        output_stream << s + 1;
    }
    output_stream << endl;
    // One line for each sequence
    for (size_t s1 = 0; s1 < n_seq; ++s1) {
        output_stream.width(4);
        output_stream << s1 + 1 << ')';
        for (size_t s2 = 0; s2 < n_seq; ++s2) {
            output_stream.width(4);
            int id = 0;
            if (total_table[s1][s2] > 0)
                id = (int)(100.0 * (double)match_table[s1][s2] / (double)total_table[s1][s2]);
            output_stream << id;
        }
        output_stream << endl;
    }
}

template<class SCORE_TYPE>
void Alignment_<SCORE_TYPE>::write_id_table(const std::string& file_name) const
{
    ofstream output_stream;
    output_stream.open(file_name.c_str());
    if (!output_stream) {
        std::string msg("Error: moltk.Alignment unable to write to id table file ");
        msg += file_name;
        throw std::runtime_error(msg.c_str());
    }
    write_id_table(output_stream);
}

template<class SCORE_TYPE>
std::string Alignment_<SCORE_TYPE>::id_table() const
{
    ostringstream output_stream;
    write_id_table(output_stream);
    return output_stream.str();
}

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE> Alignment_<SCORE_TYPE>::align(const Alignment_<SCORE_TYPE>& a2, const EString& e1, const EString& e2) const
{
    const Alignment_<SCORE_TYPE>& a1 = *this;
    // cerr << *this;
    // cerr << a2;
    // cerr << e1 << endl;
    // cerr << e2 << endl;
    Alignment_<SCORE_TYPE> result;
    // Add sequences from first alignment
    for (size_t r = 0; r < rows.size(); ++r)
    {
        Row newRow(a1.rows[r]);
        newRow.e_string = e1 * newRow.e_string;
        result.rows.push_back(newRow);
    }
    // Add sequences from second alignment
    for (size_t r = 0; r < a2.rows.size(); ++r)
    {
        Row newRow(a2.rows[r]);
        newRow.e_string = e2 * newRow.e_string;
        result.rows.push_back(newRow);
    }
    result.set_score(a1.get_score() + a2.get_score());

    return result;
}

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>& Alignment_<SCORE_TYPE>::load_fasta(std::istream& input_stream)
{
    while(! input_stream.eof()) {
        Biosequence sequence;
        EString e_string;
        sequence.load_fasta(input_stream);
        if (sequence.size() < 1) continue;
        if ( (get_number_of_columns() != 0)
          && (sequence.size() != get_number_of_columns()) )
        {
            throw std::runtime_error("Sequence length mismatch in Alignment.load_fasta()");
        }
        // Compress sequence
        Biosequence compressed_sequence(sequence);
        compressed_sequence.clear(); // empty sequence, but not description
        Biosequence::const_iterator r;
        for(r = sequence.begin(); r != sequence.end(); ++r) {
            if (r->get_one_letter_code() == '-')
                e_string << -1;
            else {
                e_string << 1;
                compressed_sequence.push_back(*r);
            }
        }
        Row row(compressed_sequence);
        row.e_string = e_string;
        rows.push_back(row);
    }
    return *this;
}

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>& Alignment_<SCORE_TYPE>::load_fasta(const std::string& file_name)
{
    ifstream fasta_stream;
    fasta_stream.open(file_name.c_str());
    if (! fasta_stream) {
        std::string msg("Error opening fasta file ");
        msg += file_name;
        throw std::runtime_error(msg.c_str());
    }
    load_fasta(fasta_stream);
    return *this;
}

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>& Alignment_<SCORE_TYPE>::load_fasta_string(const std::string& fasta_string)
{
    istringstream is(fasta_string);
    load_fasta(is);
    return *this;
}

template<class SCORE_TYPE>
const BaseBiosequence& Alignment_<SCORE_TYPE>::get_sequence(size_t index) const
{
    return rows[index].get_sequence();
}

template<class SCORE_TYPE>
const EString& Alignment_<SCORE_TYPE>::get_estring(size_t index) const
{
    return rows[index].e_string;
}

template<class SCORE_TYPE>
const SCORE_TYPE& Alignment_<SCORE_TYPE>::get_score() const 
{
    return m_score;
}

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>& Alignment_<SCORE_TYPE>::set_score(const SCORE_TYPE& s) 
{
    m_score = s;
    return *this;
}

template<class SCORE_TYPE>
std::string Alignment_<SCORE_TYPE>::repr() const
{
    std::ostringstream s;
    s << "moltk.Alignment(\"\"\"" << endl;
    write_fasta(s);
    s << endl;
    s << "\"\"\")";
    return s.str();
}


// TODO - put all this helper stuff into an AlignmentShuffler class

template<typename STATE_TYPE>
struct FiniteMarkovChain
{
    /// Generator
    STATE_TYPE emit(const STATE_TYPE& previous_state) const;
    /// Training
    FiniteMarkovChain& add_count(const STATE_TYPE& state1, const STATE_TYPE& state2, Real count = 1.0)
    {
        if (transition_counts.find(state1) == transition_counts.end())
            transition_counts[state1] = std::map<STATE_TYPE, Real>();
        if (transition_counts[state1].find(state2) == transition_counts[state1].end())
            transition_counts[state1][state2] = 0.0;
        transition_counts[state1][state2] += count;
    }

protected:
    /// transition_probabilities must sum to one for a particular start state
    std::map< STATE_TYPE, std::map<STATE_TYPE, Real> > transition_probablilities;
    /// transition_counts are used to train transition_probabilities
    std::map< STATE_TYPE, std::map<STATE_TYPE, Real> > transition_counts;
};

/// Helper comparison functor for sorting aligned sequences by weight
/// used by Alignment::shuffle()
template<class SCORE_TYPE>
struct has_lesser_weight
{
    has_lesser_weight(const Alignment_<SCORE_TYPE>& a) :a(a) {}
    bool operator()(int i, int j) {
        return a.get_row(i).sequence_weight < a.get_row(j).sequence_weight;
    }
    const Alignment_<SCORE_TYPE>& a;
};


struct ShuffleableColumn
{
    EString gap_pattern;
};


/// Create a shuffled version of this alignment to help estimate significance
template<class SCORE_TYPE>
Alignment_<SCORE_TYPE> Alignment_<SCORE_TYPE>::shuffle() const
{
    enum HydrophobicityState {
        HYDROPHOBIC_CILM,
        AROMATIC_FYW,
        SMALL_AGPST,
        POLAR_DENQKRH
    };

    enum ConservationState {
        MORE_CONSERVED,
        REGULAR_CONSERVED,
        LESS_CONSERVED
    };

    // 1.Arrange sequences in order of increasing weight
    //    initialize order to original order
    int nseq = get_number_of_sequences();
    vector<int> sequence_order(nseq);
    for (int i = 0; i < nseq; ++i)
        sequence_order[i] = i; // initialize to original order
    //    sort indices by weight
    std::sort(sequence_order.begin(), sequence_order.end(), has_lesser_weight<SCORE_TYPE>(*this));
    // TODO
    // 2.Store column data
    int ncol = get_number_of_columns();
    std::vector<ShuffleableColumn> original_columns(ncol);
    for (int s = 0; s < nseq; ++s)
    {
        const Row& row = get_row(s);
        EString::const_iterator e = row.e_string.begin();
        for (int c = 0; c < ncol; ++c)
        {
            assert(e != row.e_string.end());
            ShuffleableColumn& column = original_columns[c];
            if (*e >= 0)
                column.gap_pattern << 1;
            else
                column.gap_pattern << -1;
            // TODO
            ++e;
        }
        assert(e == row.e_string.end());
    }
    // 3.Train Markov chains for hydrophobicity and conservation
    // 4.Choose first column of shuffled alignment
    // 5.Choose subsequent columns
    // 6.Construct final shuffled alignment
    // TODO
    throw std::runtime_error("shuffle() is not implemented yet");
}


////////////////////////////
// Alignment::Row methods //
////////////////////////////

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>::Row::Row(const Biosequence& sequence_param)
    : structure(NULL)
    , sequence(new Biosequence(sequence_param))
    , type(TYPE_SEQUENCE)
    , sequence_weight(1.0)
{
    e_string << sequence->get_number_of_residues();
}

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>::Row::Row(const PDBStructure::Chain& structure_param)
    : structure(new PDBStructure::Chain(structure_param))
    , sequence(NULL)
    , type(TYPE_STRUCTURE)
    , sequence_weight(1.0)
{
    e_string << structure->get_number_of_residues();
}

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>::Row::Row(const Row& rhs)
    : structure(NULL)
    , sequence(NULL)
    , type(rhs.type)
    , sequence_weight(rhs.sequence_weight)
    , e_string(rhs.e_string)
{
    switch(type) {
        case TYPE_STRUCTURE:
            structure = new PDBStructure::Chain(*rhs.structure);
            break;
        case TYPE_SEQUENCE:
            sequence = new Biosequence(*rhs.sequence);
            break;
    }
}

template<class SCORE_TYPE>
Alignment_<SCORE_TYPE>::Row::~Row()
{
    switch(type) {
        case TYPE_STRUCTURE:
            delete structure;
            break;
        case TYPE_SEQUENCE:
            delete sequence;
            break;
    }
}

template<class SCORE_TYPE>
typename Alignment_<SCORE_TYPE>::Row& Alignment_<SCORE_TYPE>::Row::operator=(const typename Alignment_<SCORE_TYPE>::Row& rhs)
{
    structure = NULL;
    sequence = NULL;
    sequence_weight = rhs.sequence_weight;
    type = rhs.type;
    e_string = rhs.e_string;
    switch(type) {
        case TYPE_STRUCTURE:
            structure = new PDBStructure::Chain(*rhs.structure);
            break;
        case TYPE_SEQUENCE:
            sequence = new Biosequence(*rhs.sequence);
            break;
    }
    return *this;
}

template<class SCORE_TYPE>
const BaseBiosequence& Alignment_<SCORE_TYPE>::Row::get_sequence() const
{
    switch(type) {
        case TYPE_STRUCTURE:
            assert(structure);
            return *structure;
            break;
    }
    assert(sequence);
    return *sequence;
}

template<class SCORE_TYPE>
BaseBiosequence& Alignment_<SCORE_TYPE>::Row::get_sequence()
{
    switch(type) {
        case TYPE_STRUCTURE:
            assert(structure);
            return *structure;
            break;
    }
    assert(sequence);
    return *sequence;
}

template<class SCORE_TYPE>
std::string Alignment_<SCORE_TYPE>::Row::get_description() const
{
    switch(type) {
        case TYPE_STRUCTURE:
            return "structure"; // TODO
            break;
    }
    assert(sequence);
    return sequence->get_description();
}


////////////////////
// Global methods //
////////////////////

Alignment moltk::load_fasta(const std::string& file_name)
{
    Alignment result;
    result.load_fasta(file_name);
    return result;
}


// Instantiate
template class Alignment_<moltk::units::Information>;
