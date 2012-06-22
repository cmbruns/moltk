'''
Created on Jun 20, 2012

@author: brunsc
'''

from rotation import Rotation, Vec3
from math import sqrt, pi

class Trackball:
    def __init__(self):
        self.rotation = Rotation()
        self.old_pos = None
        
    def mouseMoveEvent(self, event, windowSize):
        if self.old_pos is not None:
            dx = event.pos() - self.old_pos
            x = dx.x()
            y = dx.y()
            w = (windowSize.width() + windowSize.height()) / 2.0
            # Dragging the whole window size would be 180 degrees
            angle = pi * sqrt(x*x + y*y) / w
            axis = Vec3([y, x, 0]).unit() # orthogonal to drag direction
            print axis, angle
            r = Rotation().set_from_angle_about_unit_vector(angle, axis)
            print r
        self.old_pos = event.pos()
        
    def mousePressEvent(self, event):
        self.old_pos = None
        
    def mouseReleaseEvent(self, event):
        self.old_pos = None
