from math import floor

class CatmullRomSpline:
    def _compute_sequence_indices(self, sequence, index, do_loop):
        vals = []
        i1 = int(floor(index))
        for di in [-1, 0, 1, 2]:
            i = i1 + di
            # ensure index is within range
            if do_loop: # interpolate between final and first elements
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
        return vals
    
    def interpolate_sequence(self, sequence, index, do_loop=False):
        vals = self._compute_sequence_indices(sequence, index, do_loop)
        i1 = int(floor(index))
        return self.interpolate(vals[0], vals[1], vals[2], vals[3], float(index) - i1)
    
    def interpolate_quaternion_sequence(self, sequence, index, do_loop=False):
        vals = self._compute_sequence_indices(sequence, index, do_loop)
        i1 = int(floor(index))
        return self.interpolate_quaternion(vals[0], vals[1], vals[2], vals[3], float(index) - i1)
    
    def interpolate(self, p0, p1, p2, p3, t):
        # from http://www.mvps.org/directx/articles/catmull/
        t2 = t*t
        t3 = t*t2
        return 0.5 * ( (2.0*p1) 
                   + (-p0 + p2) * t
                   + (2.0*p0 - 5.0*p1 + 4.0*p2 - p3) * t2
                   + (-p0 + 3.0*(p1 - p2) + p3) * t3 )
        
    def interpolate_quaternion(self, q00, q01, q02, q03, t):
        """
        From method CatmullQuat(...) on page 449 of Visualizing Quaternions
        """
        q10 = q00.slerp(q01, t+1.0)
        q11 = q01.slerp(q02, t+0.0)
        q12 = q02.slerp(q03, t-1.0)
        q20 = q10.slerp(q11, (t+1.0)/2.0)
        q21 = q11.slerp(q12, t/2.0)
        return q20.slerp(q21, t)
        
        