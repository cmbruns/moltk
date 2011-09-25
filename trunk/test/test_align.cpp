#include "moltk/align.h"
#include "moltk/fasta.h"
#include <iostream>

using namespace std;
using namespace moltk::align;
using namespace moltk;

int main()
{
    FastaSequence seq1("AACDEF");
    FastaSequence seq2("ACEFG");
    Aligner aligner();
    Alignment alignment = aligner.align(seq1, seq2);
    cout << alignment;

    return 0;
}
