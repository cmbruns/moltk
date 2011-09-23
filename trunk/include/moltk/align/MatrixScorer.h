#ifndef MOLTK_ALIGN_MATRIX_SCORER_H
#define MOLTK_ALIGN_MATRIX_SCORER_H

// MatrixScorer knows how to lookup residue alignment scores from 
// a matrix such as BLOSUM62
class MatrixScorer : public Scorer
{
public:
    static const MatrixScorer& getBlosum62Scorer();
};

#endif // MOLTK_ALIGN_MATRIX_SCORER_H
