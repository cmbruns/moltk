'''
Created on Jun 20, 2012

@author: brunsc
'''

from math import cos, sin, sqrt, atan2
from sys import float_info


class UnitVec3:
    def __init__(self, val=[1.0, 0.0, 0.0]):
        v = Vec3(val)
        n2 = v.normSqr()
        if (n2 <= 0):
            raise ValueError("Cannot create UnitVec3 from zero length vector")
        v /= sqrt(n2)
        self.data = (v[0], v[1], v[2])
        
    def __eq__(self, other):
        return self[:] == other[:]
    
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
    

class CoordinateAxis:
    def __init__(self, index):
        "zero for x, one for y, two for z"
        self.index = index
        
    def __eq__(self, other):
        return self.index == other.index
    
    def next_axis(self):
        next_index = self.index + 1
        while next_index > 2:
            next_index -= 3
        return CoordinateAxis._axes[next_index]

    def previous_axis(self):
        previous_index = self.index - 1
        while previous_index < 0:
            previous_index += 3
        return CoordinateAxis._axes[previous_index]

    def third_axis(self, other):
        assert(self != other)
        next = self.next_axis()
        if next == other:
            return other.next_axis()
        return next
    
    def is_reverse_cyclical(self, other):
        return self.previous_axis() == other        

CoordinateAxis._axes = [CoordinateAxis(0),
                        CoordinateAxis(1),
                        CoordinateAxis(2)]

XAxis = CoordinateAxis._axes[0]
YAxis = CoordinateAxis._axes[1]
ZAxis = CoordinateAxis._axes[2]

# Whether a particular rotation sequence is body-fixed or space-fixed
BodyRotationSequence=0
SpaceRotationSequence = 1


class Rotation:
    def __init__(self):
        self._rows = ((1.0, 0.0, 0.0),
                      (0.0, 1.0, 0.0),
                      (0.0, 0.0, 1.0))

    def __len__(self):
        return len(self._rows) # should be 3
    
    def __getitem__(self, key):
        return self._rows[key]

    def __str__(self):
        return str(self._rows)
    
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
    
    def set_from_three_angles_three_axes(self, body_or_space,
                                         angle1, axis1,
                                         angle2, axis2,
                                         angle3, axis3):
        "transcribed from simtk C++ method"
        # If axis2 is same as axis1 or axis3, efficiently calculate with a two-angle,
        # two-axis rotation.
        if axis2 == axis1: 
            return setRotationFromTwoAnglesTwoAxes(bodyOrSpace, angle1+angle2, axis1, angle3, axis3)
        if axis2 == axis3:
            return setRotationFromTwoAnglesTwoAxes(bodyOrSpace, angle1, axis1, angle2+angle3, axis3)
        # If using a SpaceRotationSequence, switch the order of the axes and the angles.
        if bodyOrSpace == SpaceRotationSequence:
            angle1, angle3 = angle3, angle1
            axis1, axis3 = axis3, axis1
        # If using a reverse cyclical, negate the signs of the angles.
        if axis1.is_reverse_cyclical(axis2):
            angle1 = -angle1
            angle2 = -angle2
            angle3 = -angle3
        # Calculate the sines and cosines (some hardware can do this more 
        # efficiently as one Taylor series).
        c1 = cos( angle1 );  s1 = sin( angle1 )
        c2 = cos( angle2 );  s2 = sin( angle2 )
        c3 = cos( angle3 );  s3 = sin( angle3 )
        # All calculations are based on a body-fixed rotation sequence.
        # Determine whether this is a BodyXYX or BodyXYZ type of rotation sequence.
        if axis1 == axis3: 
            setThreeAngleTwoAxesBodyFixedForwardCyclicalRotation(  c1,s1,axis1, c2,s2,axis2, c3,s3);
        else:
            setThreeAngleThreeAxesBodyFixedForwardCyclicalRotation(c1,s1,axis1, c2,s2,axis2, c3,s3,axis3);
        return self

    def convert_three_axes_rotation_to_three_angles(self, bodyOrSpace, axis1, axis2, axis3):
        # If all axes are same, efficiently calculate with a one-axis, one-angle method
        if axis1 == axis2 == axis3:
            theta = convertOneAxisRotationToOneAngle(axis1) / 3.0 
            return [theta,theta,theta]
        # If axis2 is same as axis1, efficiently calculate with a two-angle, two-axis rotation
        if axis2 == axis1:
            xz = convertTwoAxesRotationToTwoAngles(bodyOrSpace,axis1,axis3)
            theta = xz[0] / 2.0
            return [theta, theta, xz[1]]
        # If axis2 is same as axis3, efficiently calculate with a two-angle, two-axis rotation
        if axis2 == axis3:
            xz = convertTwoAxesRotationToTwoAngles(bodyOrSpace,axis1,axis3)
            theta = xz[1] / 2.0
            return [xz[0], theta, theta]
        # If using a SpaceRotationSequence, switch the order of the axes (later switch the angles)
        if bodyOrSpace == SpaceRotationSequence:
            axis1, axis3 = axis3, axis1
        # All calculations are based on a body-fixed rotation sequence.
        # Determine whether this is a BodyXYX or BodyXYZ type of rotation sequence.
        if axis1 == axis3:
            ans = convertTwoAxesBodyFixedRotationToThreeAngles(axis1, axis2)
        else:
            ans = self.convert_three_axes_body_fixed_rotation_to_three_angles(axis1, axis2, axis3)
        # If using a SpaceRotationSequence, switch the angles now.
        if bodyOrSpace == SpaceRotationSequence:
            ans[0], ans[2] = ans[2], ans[0]
        return ans

    def convert_three_axes_body_fixed_rotation_to_three_angles(self, axis1, axis2, axis3):
        # Ensure this method has proper arguments
        assert( axis1 != axis2 != axis3 )
        i = axis1.index
        j = axis2.index
        k = axis3.index
        # Need to know if using a forward or reverse cyclical
        plusMinus = 1.0;  minusPlus = -1.0
        if axis1.is_reverse_cyclical(axis2):
             plusMinus = -1.0;  minusPlus = 1.0
        # Shortcut to the elements of the rotation matrix
        R = self
        # Calculate theta2 using lots of information in the rotation matrix
        Rsum   =  sqrt((R[i][i]**2 + R[i][j]**2 + R[j][k]**2 + R[k][k]**2) / 2.0)
        theta2 =  atan2( plusMinus*R[i][k], Rsum ) # Rsum = abs(cos(theta2)) is inherently positive
    
        # There is a "singularity" when cos(theta2) == 0
        if Rsum > 4.0*float_info.epsilon:
            theta1 =  atan2( minusPlus*R[j][k], R[k][k] )
            theta3 =  atan2( minusPlus*R[i][j], R[i][i] )
        elif plusMinus*R[i][k] > 0.0:
            spos = R[j][i] + plusMinus*R[k][j]  # 2*sin(theta1 + plusMinus*theta3)
            cpos = R[j][j] + minusPlus*R[k][i]  # 2*cos(theta1 + plusMinus*theta3)
            theta1PlusMinusTheta3 = atan2( spos, cpos )
            theta1 = theta1PlusMinusTheta3  # Arbitrary split
            theta3 = 0.0                      # Arbitrary split
        else:
            sneg = plusMinus*(R[k][j] + minusPlus*R[j][i])  # 2*sin(theta1 + minusPlus*theta3)
            cneg = R[j][j] + plusMinus*R[k][i]              # 2*cos(theta1 + minusPlus*theta3)
            theta1MinusPlusTheta3 = atan2( sneg, cneg )
            theta1 = theta1MinusPlusTheta3  # Arbitrary split
            theta3 = 0.0                     # Arbitrary split
        # Return values have the following ranges:
        # -pi   <=  theta1  <=  +pi
        # -pi/2 <=  theta2  <=  +pi/2   (Rsum is inherently positive)
        # -pi   <=  theta3  <=  +pi
        return [theta1, theta2, theta3]

def test():
    assert 'b' is 'b'
