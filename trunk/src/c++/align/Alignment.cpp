#include "moltk/align/Alignment.h"
#include "moltk/align/Position.h"
#include "moltk/align/MatrixScorer.h"

using namespace std;
using namespace moltk;
using namespace moltk::align;
using moltk::units::bit;


///////////////////////
// Alignment methods //
///////////////////////

Alignment::Alignment()
{init();}

Alignment::Alignment(const Sequence& seq1Param, const Sequence& seq2Param)
    : gapOpenPenalty(1.0 * bit), gapExtensionPenalty(0.5 * bit)
{
    init();
    init(seq1Param, seq2Param);
}

void Alignment::init()
{
    gapOpenPenalty = 1.0 * bit; 
    gapExtensionPenalty = 0.5 * bit;
    m = seq1.size();
    n = seq2.size();
}

void Alignment::init(const Sequence& seq1Param, const Sequence& seq2Param)
{
    seq1 = seq1Param;
    seq2 = seq2Param;
    m = seq1.size();
    n = seq2.size();
}

void Alignment::align()
{
    allocate_dp_table();
    initialize_dp_table();
    compute_recurrence();
    compute_traceback();
}

void Alignment::allocate_dp_table()
{
    // TODO - small memory version allocates just 2 or 3 rows.
    dpTable.assign(m + 1, DpRow(n + 1));
}

void Alignment::initialize_dp_table()
{
    for (size_t i = 0; i < dpTable.size(); ++i)
        initialize_dp_row(i, dpTable[i]);
}

// single row initialization could come in handy during small memory alignment
void Alignment::initialize_dp_row(size_t rowIndex, DpRow& row)
{
    size_t i = rowIndex;
    // most rows only need the first element initialized
    if (bEndGapsFree)
        row[0].v = row[0].e = 0.0 * bit;
    else
        row[0].v = row[0].e = -gapOpenPenalty - ((double)i * gapExtensionPenalty);

    // ...but the top row gets full treatment
    if (rowIndex == 0)
        for (size_t j = 0; j < row.size(); ++j)
        {
            Cell& cell = row[j];
            if (bEndGapsFree)
                cell.v = cell.f = 0.0 * bit;
            else
                cell.v = cell.f = -gapOpenPenalty - ((double)j * gapExtensionPenalty);
        }
}

void Alignment::compute_recurrence()
{
    for (size_t i = 1; i <= m; ++i)
    {
        Position& p1 = *seq1[i-1];
        for (size_t j = 1; j <= n; ++j)
        {
            Position& p2 = *seq2[j-1];
            Cell& cell = dpTable[i][j];
            cell.s = p1.score(p2);
            cell.g = dpTable[i-1][j-1].v + cell.s; // score...
            cell.e = std::max(dpTable[i][j-1].e
                            , dpTable[i][j-1].v - gapOpenPenalty)
                            - gapExtensionPenalty;
            cell.f = std::max(dpTable[i-1][j].f
                            , dpTable[i-1][j].v - gapOpenPenalty)
                            - gapExtensionPenalty;
            cell.v = std::max(cell.g
                   , std::max(cell.e
                            , cell.f));
        }
    }
}

void Alignment::compute_traceback()
{
    // TODO
}

Alignment::Alignment(
          const moltk::FastaSequence& s1, 
          const moltk::FastaSequence& s2,
          const Scorer& scorer /* = getDefaultScorer() */)
{
    init(scorer.getSequence(s1), scorer.getSequence(s2));
}

/* static */
const Scorer& Alignment::getDefaultScorer() {
    return MatrixScorer::getBlosum62Scorer();
}


////////////////////
// global methods //
////////////////////

std::ostream& operator<<(std::ostream& os, const moltk::align::Alignment&)
{
    os << "sequence";
    return os;
}
