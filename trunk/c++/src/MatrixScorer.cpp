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

#include "moltk/MatrixScorer.h"
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
const MatrixScorer& MatrixScorer::getBlosum62Scorer() 
{
    static const MatrixScorer blosum62Scorer(blosum62Text);
    return blosum62Scorer;
}

/* explicit */
MatrixScorer::MatrixScorer(const std::string& matrixString) 
{
    stringstream ss (stringstream::in | stringstream::out);
    ss << blosum62Text;
    ss >> *this;
}

/* explicit */
MatrixScorer::MatrixScorer(std::istream& matrixStream)
{
    loadStream(matrixStream);
}

istream& MatrixScorer::loadStream(istream& is)
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
    characterIndices.assign(256, -1);
    while (oneLetterStream.good()) {
        std::string letterString;
        oneLetterStream >> letterString;
        // cout << "Here is a letter: " << letterString[0] << endl;
        int upperChar = std::toupper(letterString[0]);
        characterIndices[upperChar] = index;
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
        int rowIndex = characterIndices[std::toupper(sequenceLetterString[0])];
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
std::vector<POSB*> MatrixScorer::createFooPositions(const Alignment& alignment) const
{
    std::vector<POSB*> result;
    const size_t ncol = alignment.getNumberOfColumns();
    const size_t nseq = alignment.getNumberOfSequences();
    if (ncol <= 0) return result;
    // Initialize target positions with zero-scoring initial profile
    for (size_t col = 0; col <= ncol; ++col) // One more position than there are columns
    {
        POS* pos = new POS();
        // pos->scoresByResidueTypeIndex.assign(matrix.size(), 0.0 * bit);
        pos->m_gapOpenPenalty = 0.0 * bit;
        pos->m_gapExtensionPenalty = 0.0 * bit;
        result.push_back(pos);
    }
    // Add contributions from each residue of each sequence
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        // Special treatment for position zero, which corresponds to column -1
        // end gaps free?
        double gapFactor = 1.0;
        if (getEndGapsFree())
            gapFactor = 0.0; // Always starts at a left end gap
        {
            POS& pos = 
                dynamic_cast<POS&>(*result[0]);
            // leave score zero, but set gap penalties
            pos.m_gapExtensionPenalty = gapFactor * defaultGapExtensionPenalty;
            pos.m_gapOpenPenalty = gapFactor * defaultGapOpenPenalty;
        }
        int colIx = -1;
        const BaseBiosequence& seq = alignment.getSequence(seqIx);
        const Alignment::EString& eString = alignment.getEString(seqIx);
        Alignment::EString::const_iterator e;
        for (e = eString.begin(); e != eString.end(); ++e) 
        {
            size_t eResIx = *e;
            if (eResIx >= 0) { // eResIx is an actual residue number
                // Is this an end gap?
                // This cannot be a left end-gap, but it could be
                // a right end gap.
                gapFactor = 1.0;
                if (   getEndGapsFree() 
                    && (eResIx == (seq.getNumberOfResidues() - 1) ) )
                {
                    gapFactor = 0.0;
                }
                assert(eResIx <= (seq.getNumberOfResidues() - 1));
            }
            ++colIx;
            // Position[i+1] represents column i
            POS& pos = 
                dynamic_cast<POS&>(*result[colIx + 1]);
            double gapFactor = 1.0; // the usual case
            // Set gap penalties
            pos.m_gapExtensionPenalty = gapFactor * defaultGapExtensionPenalty;
            pos.m_gapOpenPenalty = gapFactor * defaultGapOpenPenalty;
        }
        assert(colIx == alignment.getNumberOfColumns() - 1);
    }
    return result;
}

// createTargetPositions() is used to precompute everything that can be precomputed
// in O(n) time, so that alignment score can be computed as quickly as possible
// durint the O(n^2) alignment phase.
/* virtual */
std::vector<Aligner::TargetPosition*> MatrixScorer::createTargetPositions(const Alignment& alignment) const
{
    std::vector<Aligner::TargetPosition*> result = 
        createFooPositions<Aligner::TargetPosition, MatrixScorer::TargetPosition>(alignment);

    const size_t ncol = alignment.getNumberOfColumns();
    // initialize
    for (size_t col = 0; col <= ncol; ++col) // One more position than there are columns
    {
        MatrixScorer::TargetPosition& pos = 
                dynamic_cast<MatrixScorer::TargetPosition&>(*result[col]);
        pos.scoresByResidueTypeIndex.assign(matrix.size(), 0.0 * bit);
    }
    const size_t nseq = alignment.getNumberOfSequences();
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        const BaseBiosequence& seq = alignment.getSequence(seqIx);
        const Alignment::EString& eString = alignment.getEString(seqIx);
        Alignment::EString::const_iterator e;
        int colIx = -1;
        for (e = eString.begin(); e != eString.end(); ++e) 
        {
            size_t eResIx = *e;
            colIx++;
            if (eResIx < 0) // This is a gap position
                continue; // gap - zero score
            // Compute alignment score
            MatrixScorer::TargetPosition& pos = 
                dynamic_cast<MatrixScorer::TargetPosition&>(*result[colIx + 1]);
            const BaseBiosequence::BaseResidue& res = seq.getResidue(eResIx);
            size_t resTypeIndex = characterIndices[res.getOneLetterCode()];
            // loop over scoring matrix positions
            for (size_t m = 0; m < matrix.size(); ++m) {
                pos.scoresByResidueTypeIndex[resTypeIndex] += matrix[resTypeIndex][m];
            }
       }
    }
    return result;
}

/* virtual */
std::vector<Aligner::QueryPosition*> MatrixScorer::createQueryPositions(const Alignment& alignment) const
{
    std::vector<Aligner::QueryPosition*> result = 
        createFooPositions<Aligner::QueryPosition, MatrixScorer::QueryPosition>(alignment);

    // queryWeightIndexByResTypeIndex helps coalesce multiple instances of the same residue in a column
    const size_t ncol = alignment.getNumberOfColumns();
    std::vector< std::map<size_t, size_t> > queryWeightIndexByResTypeIndexByColumn;
    queryWeightIndexByResTypeIndexByColumn.assign( ncol, std::map<size_t, size_t>() );

    const size_t nseq = alignment.getNumberOfSequences();
    for (size_t seqIx = 0; seqIx < nseq; ++seqIx) 
    {
        const BaseBiosequence& seq = alignment.getSequence(seqIx);
        const Alignment::EString& eString = alignment.getEString(seqIx);
        Alignment::EString::const_iterator e;
        int colIx = -1;
        for (e = eString.begin(); e != eString.end(); ++e) 
        {
            size_t eResIx = *e;
            colIx++;
            if (eResIx < 0) // This is a gap position
                continue; // gap - zero score
            // Compute alignment score
            MatrixScorer::QueryPosition& pos = 
                dynamic_cast<MatrixScorer::QueryPosition&>(*result[colIx + 1]);
            const BaseBiosequence::BaseResidue& res = seq.getResidue(eResIx);
            size_t resTypeIndex = characterIndices[res.getOneLetterCode()];
            std::map<size_t, size_t>& qmap = 
                queryWeightIndexByResTypeIndexByColumn[colIx];
            if ( qmap.find(resTypeIndex) == qmap.end() )
            {
                qmap[resTypeIndex] = pos.residueTypeIndexWeights.size();
                pos.residueTypeIndexWeights.push_back( 
                    std::pair<size_t, double>(resTypeIndex, 0.0) );
            }
            pos.residueTypeIndexWeights[qmap[resTypeIndex]].second += 1.0;
       }
    }
    return result;
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
MatrixScorer::QueryPosition* MatrixScorer::QueryPosition::clone() const
{
    return new MatrixScorer::QueryPosition(*this);
}

/* virtual */
MatrixScorer::TargetPosition* MatrixScorer::TargetPosition::clone() const
{
    return new MatrixScorer::TargetPosition(*this);
}

/* virtual */
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
    const QueryWeights& queryWeights = rhs.residueTypeIndexWeights;
    QueryWeights::const_iterator i;
    for (i = queryWeights.begin();  i != queryWeights.end(); ++i) 
    {
        double resTypeCount = i->second;
        size_t resTypeIndex = i->first;
        result += resTypeCount * lhs.scoresByResidueTypeIndex[resTypeIndex];
    }
    return result;
}

////////////////////
// Global methods //
////////////////////

istream& moltk::operator>>(istream& is, moltk::MatrixScorer& scorer)
{
    return scorer.loadStream(is);
};
