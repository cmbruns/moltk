import moltk
seq1 = moltk.FastaSequence("AACAMIAAA")
seq2 = moltk.FastaSequence("AAACAQMIAA")
aligner = moltk.Aligner()
alignment = aligner.align(seq1, seq2)
print alignment
