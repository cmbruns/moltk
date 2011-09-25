#ifndef MOLTK_ALIGN_MATRIX_SCORER_H
#define MOLTK_ALIGN_MATRIX_SCORER_H

#include "moltk/align/Scorer.h"
#include <iostream>

namespace moltk { namespace align {

// MatrixScorer knows how to lookup residue alignment scores from 
// a matrix such as BLOSUM62
class MatrixScorer : public Scorer
{
public:
    explicit MatrixScorer(const std::string& matrixString);
    explicit MatrixScorer(std::istream& matrixStream);
    virtual Alignable getSequence(const FastaSequence&) const;
    std::istream& loadStream(std::istream&);

    static const MatrixScorer& getBlosum62Scorer();
};

}} // namespace moltk::align

std::istream& operator>>(std::istream&, moltk::align::MatrixScorer&);

#endif // MOLTK_ALIGN_MATRIX_SCORER_H
