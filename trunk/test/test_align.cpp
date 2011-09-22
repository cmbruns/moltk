#include "moltk/align.h"
#include <iostream>

using namespace std;
using namespace moltk;

int main()
{
    Sequence seq1("AACDEF");
    Sequence seq2("ACEFG");
    Alignment alignment(seq1, seq2);
    alignment.align();
    cout << alignment;

    return 0;
}
