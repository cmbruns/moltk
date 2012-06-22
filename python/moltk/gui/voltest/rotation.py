'''
Created on Jun 20, 2012

@author: brunsc
'''

from math import cos, sin, sqrt


class UnitVec3:
    def __init__(self, val=[1.0, 0.0, 0.0]):
        v = Vec3(val)
        n2 = v.normSqr()
        if (n2 <= 0):
            raise ValueError("Cannot create UnitVec3 from zero length vector")
        v /= sqrt(n2)
        self.data = (v[0], v[1], v[2])
        
    def __str__(self):
        return str(self.data)

    def __repr__(self):
        return "Vec3(%s)" % repr(self.data)
    
    def __len__(self):
        return len(self.data) # should be 3
    
    def __getitem__(self, key):
        return self.data[key]
    
    def __mul__(self, rhs):
        return Vec3([self[x]*rhs for x in range(3)])
    
    def __rmul__(self, lhs):
        return Vec3([lhs*self[x] for x in range(3)])
    
    def __div__(self, rhs):
        return Vec3([self[x]/rhs for x in range(3)])
    
    def unit(self):
        return self
    
    def dot(self, rhs):
        return sum([self[x]*rhs[x] for x in range(3)])
    
    def normSqr(self):
        return self.dot(self)
    
    def norm(self):
        return sqrt(self.normSqr())


class Vec3(UnitVec3):
    def __init__(self, val=[0,0,0]):
        self.data = [1.0*val[x] for x in range(3)]
        
    def __setitem__(self, key, value):
        self.data[key] = value

    def unit(self):
        return self * 1.0 / self.norm()


class Quaternion:
    def __init__(self, angle=0.0, unit_vector=None):
        if unit_vector is None:
            self._q = (1.0, 0.0, 0.0, 0.0)
        else:
            ca2 = cos(angle/2.0)
            sa2 = sin(angle/2.0)
            if ca2 < 0:
                ca2 = -ca2
                sa2 = -sa2
            self._q = (ca2,
                       sa2 * unit_vector[0],
                       sa2 * unit_vector[1],
                       sa2 * unit_vector[2])
            
    def __len__(self):
        return len(self._q) # should be 4

    def __getitem__(self, key):
        return self._q[key]
    

class Rotation:
    def __init__(self):
        self._rows = ((1.0, 0.0, 0.0),
                      (0.0, 1.0, 0.0),
                      (0.0, 0.0, 1.0))

    def set_from_quaternion(self, q):
        q00=q[0]*q[0]; q11=q[1]*q[1]; q22=q[2]*q[2]; q33=q[3]*q[3]
        q01=q[0]*q[1]; q02=q[0]*q[2]; q03=q[0]*q[3]
        q12=q[1]*q[2]; q13=q[1]*q[3]; q23=q[2]*q[3]
        self._rows = ((q00+q11-q22-q33,   2*(q12-q03),   2*(q13+q02)),
                      ( 2*(q12+q03) , q00-q11+q22-q33,   2*(q23-q01)),
                      ( 2*(q13-q02) ,   2*(q23+q01)  , q00-q11-q22+q33))
        return self
        
    def set_from_angle_about_unit_vector(self, angle, unit_vector):
        q = Quaternion(angle, unit_vector)
        self.set_from_quaternion(q)
        return self

    def __str__(self):
        return str(self._rows)
    
    def __len__(self):
        return len(self._rows) # should be 3

def test():
    assert 'b' is 'b'
