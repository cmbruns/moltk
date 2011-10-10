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
#    For questions contact: cmbruns@rotatingpenguin.com

import moltk
seq1 = moltk.Biosequence("AACAMIAAA")
seq2 = moltk.Biosequence("AAACAQMIAA")
aligner = moltk.Aligner()
alignment = aligner.align(seq1, seq2)
print alignment
