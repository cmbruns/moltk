#include "moltk/Aligner.h"
#include "moltk/MatrixScorer.h"
#include <cassert>
#include <algorithm> // reverse
#include <limits>

using namespace std;
using namespace moltk;
using moltk::units::bit;


///////////////////////////
// Aligner::Cell methods //
///////////////////////////

units::Information Aligner::Cell::compute_v() const
{
    TracebackPointer tp = compute_traceback_pointer();
    switch(tp)
    {
    case TRACEBACK_UPLEFT:
        return g;
    case TRACEBACK_LEFT:
        return e;
    default:
        return f;
    }
}

Aligner::TracebackPointer Aligner::Cell::compute_traceback_pointer() const
{
    if ( (g >= e) && (g >= f) )
        return TRACEBACK_UPLEFT;
    else if (e >= f)
        return TRACEBACK_LEFT;
    else
        return TRACEBACK_UP;
}


/////////////////////
// Aligner methods //
/////////////////////

Aligner::Aligner()
{init();}

void Aligner::init()
{
    scorer = new MatrixScorer(MatrixScorer::getBlosum62Scorer());
    gapOpenPenalty = 1.0 * bit; 
    gapExtensionPenalty = 0.5 * bit;
    m = seq1.size();
    n = seq2.size();
}

Alignment Aligner::align(const Biosequence& s1, const Biosequence& s2)
{

    // Fill seq1, seq2
    seq1.clear();
    Biosequence::const_iterator si = s1.begin();
    while (si != s1.end()) {
        seq1.push_back(scorer->createPosition(*si));
        ++si;
    }
    m = seq1.size();

    seq2.clear();
    si = s2.begin();
    while (si != s2.end()) {
        seq2.push_back(scorer->createPosition(*si));
        ++si;
    }
    n = seq2.size();

    allocate_dp_table();
    initialize_dp_table();
    compute_recurrence();
    alignment = compute_traceback();
    return alignment;
}

void Aligner::allocate_dp_table()
{
    // TODO - small memory version allocates just 2 or 3 rows.
    dpTable.assign(m + 1, DpRow(n + 1));
}

void Aligner::initialize_dp_table()
{
    for (size_t i = 0; i < dpTable.size(); ++i)
        initialize_dp_row(i, dpTable[i]);
}

// single row initialization could come in handy during small memory alignment
void Aligner::initialize_dp_row(size_t rowIndex, DpRow& row)
{
    size_t i = rowIndex;
    // most rows only need the first element initialized
    row[0].f = numeric_limits<Real>::infinity() * bit;
    if (bEndGapsFree)
        row[0].v = row[0].e = 0.0 * bit;
    else
        row[0].v = row[0].e = -gapOpenPenalty - ((double)i * gapExtensionPenalty);

    // ...but the top row gets full treatment
    if (rowIndex == 0) {
        for (size_t j = 0; j < row.size(); ++j)
        {
            Cell& cell = row[j];
            cell.e = numeric_limits<Real>::infinity() * bit;
            if (bEndGapsFree)
                cell.v = cell.f = 0.0 * bit;
            else
                cell.v = cell.f = -gapOpenPenalty - ((double)j * gapExtensionPenalty);
        }
    }
}

void Aligner::compute_cell_recurrence(int i, int j)
{
    Position& p1 = *seq1[i-1];
    Position& p2 = *seq2[j-1];
    Cell& cell = dpTable[i][j];
    // This recurrence comes from Gusfield chapter 11.
    cell.g = dpTable[i-1][j-1].v + p1.score(p2); // score...
    cell.e = std::max(dpTable[i][j-1].e
                    , dpTable[i][j-1].v - gapOpenPenalty)
                    - gapExtensionPenalty;
    cell.f = std::max(dpTable[i-1][j].f
                    , dpTable[i-1][j].v - gapOpenPenalty)
                    - gapExtensionPenalty;
    cell.v = cell.compute_v();
}
void Aligner::compute_cell_recurrence_freeE(int i, int j)
{
    Position& p1 = *seq1[i-1];
    Position& p2 = *seq2[j-1];
    Cell& cell = dpTable[i][j];
    // This recurrence comes from Gusfield chapter 11.
    cell.g = dpTable[i-1][j-1].v + p1.score(p2); // score...
    cell.e = std::max(dpTable[i][j-1].e
                    , dpTable[i][j-1].v);
    cell.f = std::max(dpTable[i-1][j].f
                    , dpTable[i-1][j].v - gapOpenPenalty)
                    - gapExtensionPenalty;
    cell.v = cell.compute_v();

}

void Aligner::compute_cell_recurrence_freeF(int i, int j)
{
    Position& p1 = *seq1[i-1];
    Position& p2 = *seq2[j-1];
    Cell& cell = dpTable[i][j];
    // This recurrence comes from Gusfield chapter 11.
    cell.g = dpTable[i-1][j-1].v + p1.score(p2); // score...
    cell.e = std::max(dpTable[i][j-1].e
                    , dpTable[i][j-1].v - gapOpenPenalty)
                    - gapExtensionPenalty;
    cell.f = std::max(dpTable[i-1][j].f
                    , dpTable[i-1][j].v);
    cell.v = cell.compute_v();
}

void Aligner::compute_cell_recurrence_freeEF(int i, int j)
{
    Position& p1 = *seq1[i-1];
    Position& p2 = *seq2[j-1];
    Cell& cell = dpTable[i][j];
    // This recurrence comes from Gusfield chapter 11.
    cell.g = dpTable[i-1][j-1].v + p1.score(p2); // score...
    cell.e = std::max(dpTable[i][j-1].e
                    , dpTable[i][j-1].v);
    cell.f = std::max(dpTable[i-1][j].f
                    , dpTable[i-1][j].v);
    cell.v = cell.compute_v();
}

void Aligner::compute_recurrence()
{
    // If end gaps are free, final row and column have zero gap penalties.
    if (bEndGapsFree) {
        for (size_t i = 1; i < m; ++i)
        {
            for (size_t j = 1; j < n; ++j)
                compute_cell_recurrence(i, j);
            size_t j = n;
                compute_cell_recurrence_freeF(i, j);
        }
        size_t i = m;
        for (size_t j = 1; j < n; ++j)
            compute_cell_recurrence_freeE(i, j);
        size_t j = n;
        compute_cell_recurrence_freeEF(i, j);
    }
    else {
        for (size_t i = 1; i <= m; ++i)
            for (size_t j = 1; j <= n; ++j)
                compute_cell_recurrence(i, j);
    }
}

Alignment Aligner::compute_traceback()
{
    // TODO - implement end gaps free version
    Alignment result;
    result.assign(2, std::string());

    // Start at lower right of dynamic programming matrix. (TODO - only 
    // for end gaps NOT-free.
    int i = m;
    int j = n;
    // cout << "final alignment score = " << dpTable[i][j].v << endl;
    TracebackPointer tracebackPointer = dpTable[i][j].compute_traceback_pointer();
    while( (i > 0) || (j > 0) )
    {
        // cout << "traceback[" << i << "][" << j << "]" << endl;
        switch(tracebackPointer)
        {
        case TRACEBACK_UPLEFT:
            --i; --j;
            result[0].push_back( seq1[i]->getOneLetterCode() );
            result[1].push_back( seq2[j]->getOneLetterCode() );
            break;
        case TRACEBACK_UP:
            --i;
            result[0].push_back( seq1[i]->getOneLetterCode() );
            result[1].push_back( '-' );
            break;
        case TRACEBACK_LEFT:
            --j;
            result[0].push_back( '-' );
            result[1].push_back( seq2[j]->getOneLetterCode() );
            break;
        default:
            cerr << "Traceback error!" << endl;
            assert(false);
            break;
        }
        tracebackPointer = dpTable[i][j].compute_traceback_pointer();
    }
    assert(i == 0);
    assert(j == 0);
    // OK, the sequences are actually backwards here
    for (size_t i = 0; i < result.size(); ++i)
        std::reverse(result[i].begin(), result[i].end());
    return result;
}

/* static */
const Aligner::Scorer& Aligner::getDefaultScorer() {
    return MatrixScorer::getBlosum62Scorer();
}


////////////////////
// global methods //
////////////////////

