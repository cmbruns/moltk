#include "moltk/MatrixScorer.h"
#include <sstream>
#include <cassert>
#include <cctype>

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
            row[columnIndex] = value * bit;
            ++columnIndex;
        }
        // cout << "#" << endl;
    }
    return is;
}

/* virtual */
Aligner::Position* MatrixScorer::createPosition(char sequenceLetter) const
{
    MatrixScorer::Position* result = new MatrixScorer::Position();
    char letter = std::toupper(sequenceLetter);
    result->columnIndex = characterIndices[letter];
    result->scoreWeight = 1.0;
    result->rowPtr = &matrix[result->columnIndex][0];
    result->oneLetterCode = sequenceLetter;
    return result;
}


////////////////////////////////////
// MatrixScorer::Position methods //
////////////////////////////////////

/* virtual */
MatrixScorer::Position* MatrixScorer::Position::clone() const
{
    return new MatrixScorer::Position(*this);
}

/* virtual */
units::Information MatrixScorer::Position::score(const Aligner::Position& rhs) const
{
    // TODO - use a more elegant mechanism for getting the other Position
    const MatrixScorer::Position* rhsPtr = dynamic_cast<const MatrixScorer::Position*>(&rhs);
    if (! rhsPtr)
        assert(false);  // TODO
    Information result = rowPtr[rhsPtr->columnIndex] * scoreWeight * rhsPtr->scoreWeight;
    return result;
}

////////////////////
// Global methods //
////////////////////

istream& operator>>(istream& is, MatrixScorer& scorer) 
{
    return scorer.loadStream(is);
};