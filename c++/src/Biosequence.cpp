/*
 * Biosequence.cpp
 *
 *  Created on: Oct 5, 2011
 *      Author: brunsc
 */

#include "moltk/Biosequence.h"

namespace moltk
{

/////////////////////////////
// BaseBiosequence methods //
/////////////////////////////

/* virtual */
void BaseBiosequence::print_to_stream(std::ostream& os) const
{
    for(size_t i = 0; i < getNumberOfResidues(); ++i)
        os << getResidue(i).getOneLetterCode();
}


/////////////////////////
// Biosequence methods //
/////////////////////////

Biosequence::Biosequence(const std::string& str)
{
    for (size_t i = 0; i < str.length(); ++i)
        residues.push_back( Residue(str[i], i+1) );
}

Biosequence::Biosequence(const char * strParam)
{
    const std::string str(strParam);
    for (size_t i = 0; i < str.length(); ++i)
        residues.push_back( Residue(str[i], i+1) );
}

//////////////////////////////////
// Biosequence::Residue methods //
//////////////////////////////////


} // namespace moltk
