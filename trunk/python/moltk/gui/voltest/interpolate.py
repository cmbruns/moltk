from math import floor

class CatmullRomSpline:
    def interpolate_sequence(self, sequence, index, loop=False):
        vals = []
        i1 = int(floor(index))
        for di in [-1, 0, 1, 2]:
            i = i1 + di
            # ensure index is within range
            if loop: # interpolate between final and first elements
                while i < 0:
                    i += len(sequence)
                while i >= (len(sequence)):
                    i -= len(sequence)
            else: # converge to steady terminal values
                if i < 0:
                    i = 0
                if i >= len(sequence):
                    i = len(sequence) - 1
            vals.append(sequence[i])
        return self.interpolate(vals[0], vals[1], vals[2], vals[3], float(index) - i1)
    
    def interpolate(self, p0, p1, p2, p3, t):
        # from http://www.mvps.org/directx/articles/catmull/
        t2 = t*t
        t3 = t*t2
        return 0.5 * ( (2.0*p1) 
                   + (-p0 + p2) * t
                   + (2.0*p0 - 5.0*p1 + 4.0*p2 - p3) * t2
                   + (-p0 + 3.0*(p1 - p2) + p3) * t3 )
        