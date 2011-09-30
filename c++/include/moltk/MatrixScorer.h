#ifndef MOLTK_ALIGN_MATRIX_SCORER_H
#define MOLTK_ALIGN_MATRIX_SCORER_H

#include "moltk/Scorer.h"
#include "moltk/units.h"
#include <iostream>
#include <vector>

namespace moltk {

// MatrixScorer knows how to lookup residue alignment scores from 
// a matrix such as BLOSUM62
class MatrixScorer : public Scorer
{
public:
    typedef moltk::units::Information Information;

    explicit MatrixScorer(const std::string& matrixString);
    explicit MatrixScorer(std::istream& matrixStream);
    virtual Alignable getSequence(const FastaSequence&) const;
    std::istream& loadStream(std::istream&);

    static const MatrixScorer& getBlosum62Scorer();


    class MatrixPosition : public Scorer::Position
    {
    public:
        // cache values for quick score lookup
        double scoreWeight;
        int columnIndex;
        Information* rowPtr;
    };

protected:
    std::vector<int> characterIndices; // maps letters to matrix indices
    std::vector< std::vector<Information> > matrix;
};

} // namespace moltk

std::istream& operator>>(std::istream&, moltk::MatrixScorer&);

#endif // MOLTK_ALIGN_MATRIX_SCORER_H

