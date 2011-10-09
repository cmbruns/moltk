#include "moltk.h"
#include <iostream>

using namespace std;
using namespace moltk;

int main()
{
    Aligner aligner;
    aligner.setEndGapsFree(false);
    cout << aligner.align("QS", "SD");

    Biosequence seq1("AACDEF");
    Biosequence seq2("ACEFG");
    Alignment alignment = aligner.align(seq1, seq2);
    cout << alignment;

    return 0;
}

