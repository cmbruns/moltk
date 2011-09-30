#ifndef MOLTK_FASTA_SEQUENCE_H
#define MOLTK_FASTA_SEQUENCE_H

#include <string>

namespace moltk {

    class FastaSequence : public std::string
{
public:
    FastaSequence(const std::string& s) : std::string(s)
    {}

protected:
    std::string header;
};

} // namespace moltk

#endif
