#ifndef MOLTK_FASTA_SEQUENCE_H
#define MOLTK_FASTA_SEQUENCE_H

#include <string>
#include "moltk/Biosequence.h"

namespace moltk {

class FastaSequence : public Biosequence
{
public:
    FastaSequence(const std::string& s) : Biosequence(s)
    {}

protected:
    // std::string sequence;
    // std::string header;
};

} // namespace moltk

#endif
