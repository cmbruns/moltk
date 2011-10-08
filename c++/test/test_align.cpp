#include "moltk.h"
#include <iostream>

using namespace std;
using namespace moltk;

int main()
{
    Biosequence seq1("AACDEF");
    Biosequence seq2("ACEFG");
    Aligner aligner;
    Alignment alignment = aligner.align(seq1, seq2);
    cout << alignment;

    return 0;
}

