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
#include <sstream>
#include <fstream>

using namespace moltk;
using namespace std;


///////////////////////
// Alignment methods //
///////////////////////

Alignment::Alignment() 
    : m_score(0.0 * moltk::units::bit) 
    , pretty_width(50)
{}

/* implicit */
Alignment::Alignment(const Biosequence& seq) 
 : m_score(0.0 * moltk::units::bit)
 , pretty_width(50)
{
    append_sequence(seq);
}

/* implicit */ 
Alignment::Alignment(const std::string& s)
 : m_score(0.0 * moltk::units::bit)
 , pretty_width(50)
{
    load_string(s);
}

/* implicit */ 
Alignment::Alignment(const char* str)
 : m_score(0.0 * moltk::units::bit)
 , pretty_width(50)
{
    std::string s(str);
    load_string(s);
}

size_t Alignment::get_number_of_columns() const
{
    if (rows.size() == 0) return 0;
    return rows[0].e_string.total_length();
}

void Alignment::load_string(const std::string& s)
{
    Biosequence b(s);
    append_sequence(b);
}

Alignment& Alignment::append_sequence(const Biosequence& seq)
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

/* virtual */
void moltk::Alignment::print_string(std::ostream& os) const
{
    for (size_t rowIx = 0; rowIx < rows.size(); ++rowIx) 
    {
        const Row& row = rows[rowIx];
        const BaseBiosequence* seq;
        if (row.list == LIST_SEQUENCE)
            seq = &sequences[row.list_index];
        else
            seq = &structures[row.list_index];
        EString::const_iterator i = row.e_string.begin();
        while(i != row.e_string.end()) {
            int resIx = *i;
            if (resIx < 0)
                os << '-'; // gap
            else
                os << seq->get_residue(resIx).get_one_letter_code();
            ++i;
        }
        os << endl;
    }
}

void Alignment::print_pretty(std::ostream& os) const
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

Alignment Alignment::align(const Alignment& a2, const EString& e1, const EString& e2) const
{
    const Alignment& a1 = *this;
    // cerr << *this;
    // cerr << a2;
    // cerr << e1 << endl;
    // cerr << e2 << endl;
    Alignment result;
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
    result.set_score(a1.score() + a2.score());

    return result;
}

Alignment& Alignment::load_fasta(std::istream& input_stream)
{
    while(! input_stream.eof()) {
        Biosequence sequence;
        EString e_string;
        sequence.load_fasta(input_stream);
        if (sequence.size() < 1) continue;
        if ( (get_number_of_columns() != 0)
          && (sequence.size() != get_number_of_columns()) )
        {
            throw std::exception("Sequence length mismatch in Alignment.load_fasta()");
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

Alignment& Alignment::load_fasta(const std::string& file_name)
{
    ifstream fasta_stream;
    fasta_stream.open(file_name.c_str());
    if (! fasta_stream) {
        std::string msg("Error opening fasta file ");
        msg += file_name;
        throw std::exception(msg.c_str());
    }
    load_fasta(fasta_stream);
    return *this;
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

