#ifndef MOLTK_FASTA_SEQUENCE_H
#define MOLTK_FASTA_SEQUENCE_H

#include <string>

namespace moltk {

class FastaSequence
{
public:
    FastaSequence(const std::string& s) : sequence(s)
    {}

protected:
    std::string header;
    std::string sequence;
};

} // namespace moltk

#endif
