#    MolTK is a Python and C++ toolkit for protein sequence/structure alignment and visualization
#    Copyright (C) 2011  Christopher M. Bruns
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#   
#    Commercial users should ask about our dual licensing model.
#    For questions contact: cmbruns@rotatingpenguin.com

# Synopsis sketch of python API for moltk alignment, like sequoia
from moltk import Aligner, Sequence

aligner = Aligner()
alignment = aligner.align(Sequence("AACDEF"))
alignment = aligner.align(Sequence("ACEFG"))
print alignment

# Mirroring tutorial example 1
python                               # (no analogy)
from moltk import Aligner, Sequence  # (no analogy)
seq1 = Sequence(file("test1.seq"))   # read SEQ1 test1.seq
seq2 = Sequence(file("test2.seq"))   # read SEQ2 test2.seq
align = Aligner().align(seq1, seq2)  # ALIGN
print align                          # print ALIGN
seq1 = align                         # set SEQ1 ALIGN
seq2 = Sequence(file("test3.seq"))   # read SEQ2 test3.seq
align = Aligner().align(seq1, seq2)  # ALIGN
print align                          # print ALIGN
print align.id()                     # print id ALIGN
quit()                               # quit

# More compactly
from moltk import Aligner, Sequence
aligner = Aligner()
aligner.align(Sequence(file("test1.seq"))
aligner.align(Sequence(file("test2.seq"))
align = aligner.align(Sequence(file("test3.seq"))
print align
print align.id()

# Example 2
import moltk
from moltk import Aligner, Sequence, Structure
struct1 = Structure().load("test1.pdb")
struct2 = Structure().load("test2.pdb")
alignment = Aligner().align(struct1, struct2, moltk.ALIGN_SEQUENCE_ONLY)
overlay = Aligner.overlay(alignment)
# TODO dcutoff


