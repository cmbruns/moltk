
/*! \mainpage MolTK: Python and C++ tools for protein sequence/structure alignment and visualization

\section whatismoltk What is MolTK?

There are three faces of MolTK; in order from easiest-to-use to most-powerful:

    -# MolTK is a computer application that allows the user to view and align molecular sequences and structures.
    -# moltk is a Python programming language module that allows the user to align and otherwise compute on molecular sequences and structures.
    -# moltk is a C++ API that allow the programmer to compute with molecular sequences and structures with high efficiency. 

\section designgoals Design Goals of MolTK

    - Python programming language interactive environment for sequence/structure alignment that is just as easy to use as our (aging) dedicated alignment tool SEQUOIA.
    - Flexible architecture that makes it easy to experiment with custom alignment methods and scoring systems.
    - Consistent, well docomented API for both Python and C++ programmers.
    - Units-aware quantity type system. Thus alignment scores are not just numbers, they are information quantities with units of "bits". Atomic coordinates are not just numbers x, y, z, but are vector quantities with units of nanometers. Units-aware quantity types are an important part of scientific computing hygiene:
        - Converting a "quantity" to a raw number requires a "unit" to express the quantity in. This requires the user to pay attention to units at precisely the moment when knowing the unit is most important. The rest of the time is "just works", even if you are wrong about what the current units are!
        - This sort of type safety in scientific computing might help prevent errors such as, say, crashing $100 million orbiters into planets.
        - Type safety: Adding a length to a volume makes no sense, and will result in an error. Dividing a length by a time results in a velocity. 
    - Next-generation molecular sequence/structure viewer that adheres to our user interface principles. 
*/

#include "moltk/Biosequence.h"
#include "moltk/FastaSequence.h"
#include "moltk/Aligner.h"
#include "moltk/Structure.h"
