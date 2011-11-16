/*
    MolTK is a Python and C++ toolkit for protein sequence/structure alignment and visualization
    Copyright (C) 2011  Christopher M. Bruns

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    
    Commercial users should ask about our dual licensing model.
    For questions contact: cmbruns@rotatingpenguin.com
*/

#include "moltk/Aligner.hpp"
#include "moltk/MatrixScorer.hpp"
#include "moltk/EString.hpp"
#include <cassert>
#include <limits>

using namespace std;
using namespace moltk;
using moltk::units::bit;


/////////////////////
// Aligner methods //
/////////////////////

Aligner::Aligner()
    : scorer(SubstitutionMatrix::get_blosum62())
{}


Alignment Aligner::align(const Alignment& s1, const Alignment& s2)
{
    return test_table.align(s1, s2, scorer);
}


/* static */
Aligner& Aligner::get_shared_aligner()
{
    static std::auto_ptr<Aligner> shared_aligner(NULL);
    if (!shared_aligner.get())
        shared_aligner.reset( new Aligner() );
    return *shared_aligner.get();
}


////////////////////
// global methods //
////////////////////

/*
 * global align() methods helps get SEQUOIA-like conciseness in python.
 */
Alignment moltk::align(const Alignment& target_alignment, const Alignment& query_alignment)
{
    return Aligner::get_shared_aligner().align(target_alignment, query_alignment);
}
