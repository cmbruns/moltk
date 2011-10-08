import moltk
seq1 = moltk.Biosequence("AACAMIAAA")
seq2 = moltk.Biosequence("AAACAQMIAA")
aligner = moltk.Aligner()
alignment = aligner.align(seq1, seq2)
print alignment
