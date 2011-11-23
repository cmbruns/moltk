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
    sequences.push_back(seq);
    Row row = {
        LIST_SEQUENCE,
        0,
        1.0,
        EString().append_run(seq.size()) };
    rows.push_back(row);
    return *this;
}

template<class SCORE_TYPE>
void Alignment_<SCORE_TYPE>::write_fasta(std::ostream& output_stream) const
{
    for (size_t rowIx = 0; rowIx < rows.size(); ++rowIx) 
    {
        const Row& row = rows[rowIx];
        const BaseBiosequence* seq;
        if (row.list == LIST_SEQUENCE) {
            seq = &sequences[row.list_index];
            output_stream << '>' << sequences[row.list_index].get_description() << endl;
        }
        else {
            seq = &structures[row.list_index];
            output_stream << "> structure" << endl;
        }
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
        if (row.list == LIST_SEQUENCE) {
            os << sequences[row.list_index].get_description();
        }
        else
            os << "structure";
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
            if (row.list == LIST_SEQUENCE)
                seq = &sequences[row.list_index];
            else
                seq = &structures[row.list_index];
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
        const Row& row = rows[r];
        Row newRow(row);
        newRow.e_string = e1 * newRow.e_string;
        if (row.list == LIST_SEQUENCE) {
            result.sequences.push_back(sequences[row.list_index]);
            assert(result.sequences.size() - 1 == newRow.list_index);
        }
        else {
            assert(row.list == LIST_STRUCTURE);
            result.structures.push_back(structures[row.list_index]);
            assert(result.structures.size() - 1 == newRow.list_index);
        }
        result.rows.push_back(newRow);
    }
    // Add sequences from second alignment
    for (size_t r = 0; r < a2.rows.size(); ++r)
    {
        const Row& row = a2.rows[r];
        Row newRow(row);
        newRow.e_string = e2 * newRow.e_string;
        if (row.list == LIST_SEQUENCE) {
            result.sequences.push_back(a2.sequences[row.list_index]);
            newRow.list_index = result.sequences.size() - 1;
        }
        else {
            assert(row.list == LIST_STRUCTURE);
            result.structures.push_back(a2.structures[row.list_index]);
            newRow.list_index = result.structures.size() - 1;
        }
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
        Row row;
        row.list = LIST_SEQUENCE;
        row.list_index = sequences.size();
        row.sequence_weight = 1.0; // TODO
        row.e_string = e_string;
        sequences.push_back(compressed_sequence);
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
    const Row& row = rows[index];
    if (row.list == LIST_SEQUENCE)
        return sequences[row.list_index];
    else
        return structures[row.list_index];
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

/// Create a shuffled version of this alignment to help estimate significance
template<class SCORE_TYPE>
Alignment_<SCORE_TYPE> Alignment_<SCORE_TYPE>::shuffle() const
{
    // TODO
    // 1 - Arrange sequences in order of increasing weight
    // 2 - Store column data
    // 3 - Train Markov chains for hydrophobicity and conservation
    // 4 - Choose first column of shuffled alignment
    // 5 - Choose subsequence columns
    // 6 - Construct final shuffled alignment
    // TODO
    throw std::runtime_error("shuffle() is not implemented yet");
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
