from interpolate import CatmullRomSpline
import unittest

class TestSpline(unittest.TestCase):
    def setUp(self):
        self.s = CatmullRomSpline()
        self.v1 = [0,1,2,3,4,5]
        
    def test_spot1(self):
        self.assertAlmostEqual(3.7, self.s.interpolate_sequence(self.v1, 3.7))
        
    def test_integers(self):
        for i in range(len(self.v1)):
            self.assertAlmostEqual(self.v1[i], self.s.interpolate_sequence(self.v1, i))


if __name__ == '__main__':
    unittest.main()
