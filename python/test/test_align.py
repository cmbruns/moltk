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


