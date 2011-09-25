#ifndef MOLTK_ALIGN_SCORER_H
#define MOLTK_ALIGN_SCORER_H

#include "moltk/align/Alignable.h"
#include "moltk/fasta.h"

namespace moltk { namespace align {

// align::Scorer is an abstract base class for objects
// that decouple protein sequences/structures from the 
// details of sequence alignment.  An align::Scorer is able
// to convert a protein sequence or structure into an
// align::Alignable of align::Postions.  Those resulting 
// align::Positions know how to compute the alignment scores
// of themselves with other align::Positions.
class Scorer
{
public:
    virtual Alignable getSequence(const FastaSequence&) const = 0;


    class Position
    {
    };
};

}} // namespace moltk::align

#endif
