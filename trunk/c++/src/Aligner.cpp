#include "moltk/Aligner.h"
#include "moltk/MatrixScorer.h"
#include <cassert>
#include <algorithm> // reverse

using namespace std;
using namespace moltk;
using moltk::units::bit;


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

Alignment Aligner::align(const FastaSequence& s1, const FastaSequence& s2)
{

    // Fill seq1, seq2
    seq1.clear();
    FastaSequence::const_iterator si = s1.begin();
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
    row[0].tracebackPointer = TRACEBACK_UP;
    if (bEndGapsFree)
        row[0].v = row[0].e = 0.0 * bit;
    else
        row[0].v = row[0].e = -gapOpenPenalty - ((double)i * gapExtensionPenalty);

    // ...but the top row gets full treatment
    if (rowIndex == 0) {
        for (size_t j = 0; j < row.size(); ++j)
        {
            Cell& cell = row[j];
            cell.tracebackPointer = TRACEBACK_LEFT;
            if (bEndGapsFree)
                cell.v = cell.f = 0.0 * bit;
            else
                cell.v = cell.f = -gapOpenPenalty - ((double)j * gapExtensionPenalty);
        }
        row[0].tracebackPointer = TRACEBACK_DONE; // upper left cell
    }
}

void Aligner::compute_recurrence()
{
    // If end gaps are free, final row and column have zero gap penalties.
    Information iGapOpenPenalty;
    Information iGapExtensionPenalty;
    Information jGapOpenPenalty;
    Information jGapExtensionPenalty;
    for (size_t i = 1; i <= m; ++i)
    {
        if (bEndGapsFree && (i == m)) {
            iGapOpenPenalty = 0.0 * bit;
            iGapExtensionPenalty = 0.0 * bit;
        }
        else {
            iGapOpenPenalty = gapOpenPenalty;
            iGapExtensionPenalty = gapExtensionPenalty;
        }
        Position& p1 = *seq1[i-1];
        for (size_t j = 1; j <= n; ++j)
        {
            if (bEndGapsFree && (j == n)) {
                jGapOpenPenalty = 0.0 * bit;
                jGapExtensionPenalty = 0.0 * bit;
            }
            else {
                jGapOpenPenalty = gapOpenPenalty;
                jGapExtensionPenalty = gapExtensionPenalty;
            }
            Position& p2 = *seq2[j-1];
            Cell& cell = dpTable[i][j];
            cell.s = p1.score(p2);
            // This recurrence comes from Gusfield chapter 11.
            cell.g = dpTable[i-1][j-1].v + cell.s; // score...
            cell.e = std::max(dpTable[i][j-1].e
                            , dpTable[i][j-1].v - iGapOpenPenalty)
                            - iGapExtensionPenalty;
            cell.f = std::max(dpTable[i-1][j].f
                            , dpTable[i-1][j].v - jGapOpenPenalty)
                            - jGapExtensionPenalty;
            // TODO compute traceback pointer
            if ( (cell.g >= cell.e) && (cell.g >= cell.f) ) {
                cell.v = cell.g;
                cell.tracebackPointer = TRACEBACK_UPLEFT;
            }
            else if (cell.e >= cell.f) {
                cell.v = cell.e;
                cell.tracebackPointer = TRACEBACK_LEFT;
            }
            else {
                cell.v = cell.f;
                cell.tracebackPointer = TRACEBACK_UP;
            }
        }
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
    TracebackPointer tracebackPointer = dpTable[i][j].tracebackPointer;
    while(tracebackPointer != TRACEBACK_DONE) 
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
        tracebackPointer = dpTable[i][j].tracebackPointer;
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

