import moltk

seq1 = moltk.FastaSequence ("AHLRVNDQACAMIA")
seq2 = moltk.FastaSequence("AAHLRVNDQACALIAAAAAAAAAAAAAAAACAMIA")
aligner = moltk.Aligner()
aligner.endGapsFree = True
alignment1 = aligner.align(seq1, seq2)
aligner.endGapsFree = False
alignment2 = aligner.align(seq1, seq2)
print alignment1
print alignment2

seq1 = moltk.FastaSequence ("AHLRVNDQACAMIA")
seq2 = moltk.FastaSequence("AAHLRVNDQAAAAAAAAAAAAHLKVNDQACAMIA")
aligner = moltk.Aligner()
aligner.endGapsFree = True
alignment1 = aligner.align(seq1, seq2)
aligner.endGapsFree = False
alignment2 = aligner.align(seq1, seq2)
print alignment1
print alignment2
