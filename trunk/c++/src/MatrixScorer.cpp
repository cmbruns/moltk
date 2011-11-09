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

#include "moltk/MatrixScorer.hpp"
#include <sstream>
#include <cassert>
#include <cctype>
#include <map>

using namespace std;
using namespace moltk;
using moltk::units::bit;

static const char* blosum62Text = "\
#  Matrix made by matblas from blosum62.iij\n\
#  * column uses minimum score\n\
#  BLOSUM Clustered Scoring Matrix in 1/2 Bit Units\n\
#  Blocks Database = /data/blocks_5.0/blocks.dat\n\
#  Cluster Percentage: >= 62\n\
#  Entropy =   0.6979, Expected =  -0.5209\n\
   A  R  N  D  C  Q  E  G  H  I  L  K  M  F  P  S  T  W  Y  V  B  Z  X  *\n\
A  4 -1 -2 -2  0 -1 -1  0 -2 -1 -1 -1 -1 -2 -1  1  0 -3 -2  0 -2 -1  0 -4 \n\
R -1  5  0 -2 -3  1  0 -2  0 -3 -2  2 -1 -3 -2 -1 -1 -3 -2 -3 -1  0 -1 -4 \n\
N -2  0  6  1 -3  0  0  0  1 -3 -3  0 -2 -3 -2  1  0 -4 -2 -3  3  0 -1 -4 \n\
D -2 -2  1  6 -3  0  2 -1 -1 -3 -4 -1 -3 -3 -1  0 -1 -4 -3 -3  4  1 -1 -4 \n\
C  0 -3 -3 -3  9 -3 -4 -3 -3 -1 -1 -3 -1 -2 -3 -1 -1 -2 -2 -1 -3 -3 -2 -4 \n\
Q -1  1  0  0 -3  5  2 -2  0 -3 -2  1  0 -3 -1  0 -1 -2 -1 -2  0  3 -1 -4 \n\
E -1  0  0  2 -4  2  5 -2  0 -3 -3  1 -2 -3 -1  0 -1 -3 -2 -2  1  4 -1 -4 \n\
G  0 -2  0 -1 -3 -2 -2  6 -2 -4 -4 -2 -3 -3 -2  0 -2 -2 -3 -3 -1 -2 -1 -4 \n\
H -2  0  1 -1 -3  0  0 -2  8 -3 -3 -1 -2 -1 -2 -1 -2 -2  2 -3  0  0 -1 -4 \n\
I -1 -3 -3 -3 -1 -3 -3 -4 -3  4  2 -3  1  0 -3 -2 -1 -3 -1  3 -3 -3 -1 -4 \n\
L -1 -2 -3 -4 -1 -2 -3 -4 -3  2  4 -2  2  0 -3 -2 -1 -2 -1  1 -4 -3 -1 -4 \n\
K -1  2  0 -1 -3  1  1 -2 -1 -3 -2  5 -1 -3 -1  0 -1 -3 -2 -2  0  1 -1 -4 \n\
M -1 -1 -2 -3 -1  0 -2 -3 -2  1  2 -1  5  0 -2 -1 -1 -1 -1  1 -3 -1 -1 -4 \n\
F -2 -3 -3 -3 -2 -3 -3 -3 -1  0  0 -3  0  6 -4 -2 -2  1  3 -1 -3 -3 -1 -4 \n\
P -1 -2 -2 -1 -3 -1 -1 -2 -2 -3 -3 -1 -2 -4  7 -1 -1 -4 -3 -2 -2 -1 -2 -4 \n\
S  1 -1  1  0 -1  0  0  0 -1 -2 -2  0 -1 -2 -1  4  1 -3 -2 -2  0  0  0 -4 \n\
T  0 -1  0 -1 -1 -1 -1 -2 -2 -1 -1 -1 -1 -2 -1  1  5 -2 -2  0 -1 -1  0 -4 \n\
W -3 -3 -4 -4 -2 -2 -3 -2 -2 -3 -2 -3 -1  1 -4 -3 -2 11  2 -3 -4 -3 -2 -4 \n\
Y -2 -2 -2 -3 -2 -1 -2 -3  2 -1 -1 -2 -1  3 -3 -2 -2  2  7 -1 -3 -2 -1 -4 \n\
V  0 -3 -3 -3 -1 -2 -2 -3 -3  3  1 -2  1 -1 -2 -2  0 -3 -1  4 -3 -2 -1 -4 \n\
B -2 -1  3  4 -3  0  1 -1  0 -3 -4  0 -3 -3 -2  0 -1 -4 -3 -3  4  1 -1 -4 \n\
Z -1  0  0  1 -3  3  4 -2  0 -3 -3  1 -1 -3 -1  0 -1 -3 -2 -2  1  4 -1 -4 \n\
X  0 -1 -1 -1 -2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -2  0  0 -2 -1 -1 -1 -1 -1 -4 \n\
* -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4 -4  1 \n\
";

/* static */
const MatrixScorer& MatrixScorer::get_blosum62_scorer() 
{
    static const MatrixScorer blosum62Scorer(blosum62Text);
    return blosum62Scorer;
}

/* explicit */
MatrixScorer::MatrixScorer(const std::string& matrixString) 
{
    istringstream ss(blosum62Text);
    ss >> *this;
}

/* explicit */
MatrixScorer::MatrixScorer(std::istream& matrixStream)
{
    load_stream(matrixStream);
}

istream& MatrixScorer::load_stream(istream& is)
{
    char lineBuffer[1000];
    // skip header comments
    while (is.good()) {
        is.getline(lineBuffer, 999);
        std::string line(lineBuffer);
        if (line[0] == '#') 
            continue;
        if (line.length() < 5)
            continue;
        break; // finished with header
    }
    // cout << lineBuffer << endl;
    // Parse list of characters supported by this matrix into 
    // characterIndices array.
    stringstream oneLetterStream(lineBuffer);
    int index = 0;
    character_indices.assign(256, -1);
    while (oneLetterStream.good()) {
        std::string letterString;
        oneLetterStream >> letterString;
        // cout << "Here is a letter: " << letterString[0] << endl;
        int upperChar = std::toupper(letterString[0]);
        character_indices[upperChar] = index;
        ++index;
    }
    // Allocate matrix
    int nChars = index;
    // std::vector< std::vector<Information> > matrix;
    matrix.assign( nChars, std::vector<Information>(nChars, 0.0 * bit) );
    // Read entries into matrix from stream
    while (is.good()) {
        is.getline(lineBuffer, 999);
        std::string lineString(lineBuffer);
        if (lineString.length() < 5) continue;
        stringstream lineStream(lineString);
        std::string sequenceLetterString;
        lineStream >> sequenceLetterString;
        int rowIndex = character_indices[std::toupper(sequenceLetterString[0])];
        std::vector<Information>& row = matrix[rowIndex];
        // cout << sequenceLetterString << " : " << rowIndex << endl;
        double value;
        size_t columnIndex = 0;
        while (lineStream.good() && (columnIndex < row.size())) {
            lineStream >> value;
            // cout << "  " << value << " ";
            // cout << columnIndex << " " << row.size() << endl;
            // Matrices are usually in 1/2 bit units...
            row[columnIndex] = value * 0.5 * bit;
            ++columnIndex;
        }
        // cout << "#" << endl;
    }
    return is;
}

template<class POSB, class POS>
std::vector<POSB*> MatrixScorer::create_foo_positions(const Alignment& alignment) const
{
    std::vector<POSB*> result;
    const size_t ncol = alignment.get_number_of_columns();
    const size_t nseq = alignment.get_number_of_sequences();
    if (ncol <= 0) return result;
    // Initialize target positions with zero-scoring initial profile
    for (size_t col = 0; col <= ncol; ++col) // One more position than there are columns
    {
        POS* pos = new POS();
        // pos->scoresByResidueTypeIndex.assign(matrix.size(), 0.0 * bit);
        pos->gap_scorer.open_penalty = 0.0 * bit;
        pos->gap_scorer.extension_penalty = 0.0 * bit;
        pos->index = col;
        result.push_back(pos);
    }
    // Add contributions from each residue of each sequence
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        // Special treatment for position zero, which corresponds to column -1
        // end gaps free?
        double gapFactor = 1.0;
        if (get_end_gaps_free())
            gapFactor = 0.0; // Always starts at a left end gap
        {
            POS& pos = 
                dynamic_cast<POS&>(*result[0]);
            // leave score zero, but set gap penalties
            pos.gap_scorer.extension_penalty = gapFactor * default_gap_extension_penalty;
            pos.gap_scorer.open_penalty = gapFactor * default_gap_open_penalty;
        }
        int colIx = -1;
        const BaseBiosequence& seq = alignment.get_sequence(seqIx);
        const EString& eString = alignment.get_estring(seqIx);
        EString::const_iterator e;
        for (e = eString.begin(); e != eString.end(); ++e) 
        {
            int eResIx = *e;
            if (eResIx >= 0) { // eResIx is an actual residue number
                // Is this an end gap?
                // This cannot be a left end-gap, but it could be
                // a right end gap.
                gapFactor = 1.0;
                if (   get_end_gaps_free() 
                    && (eResIx >= ((int)seq.get_number_of_residues() - 1) ) )
                {
                    gapFactor = 0.0;
                }
                assert(eResIx <= ((int)seq.get_number_of_residues() - 1));
            }
            ++colIx;
            // Position[i+1] represents column i
            POS& pos = 
                dynamic_cast<POS&>(*result[colIx + 1]);
            // Set gap penalties
            pos.gap_scorer.extension_penalty = gapFactor * default_gap_extension_penalty;
            pos.gap_scorer.open_penalty = gapFactor * default_gap_open_penalty;
        }
        assert(colIx == alignment.get_number_of_columns() - 1);
    }
    return result;
}

// createTargetPositions() is used to precompute everything that can be precomputed
// in O(n) time, so that alignment score can be computed as quickly as possible
// durint the O(n^2) alignment phase.
/* virtual */
std::vector<const Aligner::TargetPosition*> MatrixScorer::create_target_positions(const Alignment& alignment) const
{
    std::vector<Aligner::TargetPosition*> result =
        create_foo_positions<Aligner::TargetPosition, MatrixScorer::TargetPosition>(alignment);

    const size_t ncol = alignment.get_number_of_columns();
    // initialize
    for (size_t col = 0; col <= ncol; ++col) // One more position than there are columns
    {
        MatrixScorer::TargetPosition& pos = 
                dynamic_cast<MatrixScorer::TargetPosition&>(*result[col]);
        pos.scores_by_residue_type_index.assign(matrix.size(), 0.0 * bit);
    }
    const size_t nseq = alignment.get_number_of_sequences();
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        const BaseBiosequence& seq = alignment.get_sequence(seqIx);
        const EString& eString = alignment.get_estring(seqIx);
        EString::const_iterator e;
        int colIx = -1;
        for (e = eString.begin(); e != eString.end(); ++e) 
        {
            int eResIx = *e;
            colIx++;
            if (eResIx < 0) // This is a gap position
                continue; // gap - zero score
            // Compute alignment score
            MatrixScorer::TargetPosition& pos = 
                dynamic_cast<MatrixScorer::TargetPosition&>(*result[colIx + 1]);
            const BaseBiosequence::BaseResidue& res = seq.get_residue(eResIx);
            size_t resTypeIndex = character_indices[res.get_one_letter_code()];
            // loop over scoring matrix positions
            for (size_t m = 0; m < matrix.size(); ++m) {
                pos.scores_by_residue_type_index[m] += matrix[resTypeIndex][m];
            }
            // cerr << pos.scoresByResidueTypeIndex[17] << endl;
       }
    }

    // Copy vector of non-const pointer to const pointer, now that we are done modifying them.
    std::vector<const Aligner::TargetPosition*> result1;
    for (size_t i = 0; i < result.size(); ++i)
        result1.push_back(result[i]);
    return result1;
}

/* virtual */
std::vector<const Aligner::QueryPosition*> MatrixScorer::create_query_positions(const Alignment& alignment) const
{
    std::vector<Aligner::QueryPosition*> result =
        create_foo_positions<Aligner::QueryPosition, MatrixScorer::QueryPosition>(alignment);

    // queryWeightIndexByResTypeIndex helps coalesce multiple instances of the same residue in a column
    const size_t ncol = alignment.get_number_of_columns();
    std::vector< std::map<size_t, size_t> > queryWeightIndexByResTypeIndexByColumn;
    queryWeightIndexByResTypeIndexByColumn.assign( ncol, std::map<size_t, size_t>() );

    const size_t nseq = alignment.get_number_of_sequences();
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        const BaseBiosequence& seq = alignment.get_sequence(seqIx);
        const EString& eString = alignment.get_estring(seqIx);
        EString::const_iterator e;
        int colIx = -1;
        for (e = eString.begin(); e != eString.end(); ++e) 
        {
            int eResIx = *e; // NOT size_t, could be -1!
            colIx++;
            if (eResIx < 0) // This is a gap position
                continue; // gap - zero score
            // Compute alignment score
            MatrixScorer::QueryPosition& pos = 
                dynamic_cast<MatrixScorer::QueryPosition&>(*result[colIx + 1]);
            const BaseBiosequence::BaseResidue& res = seq.get_residue(eResIx);
            size_t resTypeIndex = character_indices[res.get_one_letter_code()];
            std::map<size_t, size_t>& qmap = 
                queryWeightIndexByResTypeIndexByColumn[colIx];
            if ( qmap.find(resTypeIndex) == qmap.end() )
            {
                qmap[resTypeIndex] = pos.residue_type_index_weights.size();
                pos.residue_type_index_weights.push_back( 
                    std::pair<size_t, double>(resTypeIndex, 0.0) );
            }
            pos.residue_type_index_weights[qmap[resTypeIndex]].second += 1.0;
       }
    }

    /*
    for (size_t col = 0; col <= ncol; ++col) // One more position than there are columns
    {
        MatrixScorer::QueryPosition& pos = 
            dynamic_cast<MatrixScorer::QueryPosition&>(*result[col]);
        // cerr << "query position " << col << ": " << pos << endl;
    }
    */

    // Copy vector of non-const pointer to const pointer, now that we are done modifying them.
    std::vector<const Aligner::QueryPosition*> result1;
    for (size_t i = 0; i < result.size(); ++i)
        result1.push_back(result[i]);
    return result1;
}

/* virtual */
/*
Aligner::Position* MatrixScorer::createPosition(char sequenceLetter, bool bTerminus) const
{
    MatrixScorer::Position* result = new MatrixScorer::Position();
    char letter = std::toupper(sequenceLetter);
    result->columnIndex = characterIndices[letter];
    result->scoreWeight = 1.0;
    result->rowPtr = &matrix[result->columnIndex][0];
    result->oneLetterCode = sequenceLetter;
    if (endGapsFree && bTerminus) {
        result->m_gapOpenPenalty = 0.0 * bit;
        result->m_gapExtensionPenalty = 0.0 * bit;
    } else {
        result->m_gapOpenPenalty = 1.0 * bit;
        result->m_gapExtensionPenalty = 0.5 * bit;
    }
    return result;
}
*/

////////////////////////////////////
// MatrixScorer::Position methods //
////////////////////////////////////

/* virtual */
/*
MatrixScorer::QueryPosition* MatrixScorer::QueryPosition::clone() const
{
    return new MatrixScorer::QueryPosition(*this);
}
*/

/* virtual */
/*
MatrixScorer::TargetPosition* MatrixScorer::TargetPosition::clone() const
{
    return new MatrixScorer::TargetPosition(*this);
}
*/

/* virtual */
/*
units::Information MatrixScorer::TargetPosition::score(const Aligner::QueryPosition& rhsParam) const
{
    const TargetPosition& lhs = *this;
    // TODO - use a more elegant mechanism for getting the other Position
    const MatrixScorer::QueryPosition* rhsPtr = dynamic_cast<const MatrixScorer::QueryPosition*>(&rhsParam);
    if (! rhsPtr)
        assert(false);  // TODO
    const MatrixScorer::QueryPosition& rhs = *rhsPtr;
    Information result = 0.0 * bit;
    typedef MatrixScorer::QueryPosition::QueryWeights QueryWeights;
    const QueryWeights& queryWeights = rhs.residue_type_index_weights;
    QueryWeights::const_iterator i;
    for (i = queryWeights.begin();  i != queryWeights.end(); ++i) 
    {
        double resTypeCount = i->second;
        size_t resTypeIndex = i->first;
        // cerr << "query weight count = " << resTypeCount;
        // cerr << " index = " << resTypeIndex << endl;
        result += resTypeCount * lhs.scores_by_residue_type_index[resTypeIndex];
    }
    return result;
}
*/

////////////////////
// Global methods //
////////////////////

istream& moltk::operator>>(istream& is, moltk::MatrixScorer& scorer)
{
    return scorer.load_stream(is);
};
