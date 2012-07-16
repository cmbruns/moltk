from actor import Actor
from shader import SphereImposter, SphereImposterArray, VertexArrayTest
from rotation import Vec3
from OpenGL.GL import *
from OpenGL.GLUT import *

class GlutSphereActor(Actor):
    def __init__(self, position=[0,0,0], radius=1.0):
        self.position = position[:]
        self.radius = radius
        
    def paint_gl(self):
        glMatrixMode(GL_MODELVIEW)
        glPushMatrix()
        p = self.position
        glTranslate(p[0], p[1], p[2])
        glutSolidSphere(self.radius, 20, 20)
        glPopMatrix()


class TeapotActor(Actor):
    def paint_gl(self):
        glPushAttrib(GL_POLYGON_BIT) # remember current GL_FRONT_FACE indictor
        glFrontFace(GL_CW) # teapot polygon vertex order is opposite to modern convention
        glColor3f(0.3, 0.3, 0.3) # paint it gray
        glutSolidTeapot(1.0) # thank you GLUT tool kit
        glPopAttrib() # restore GL_FRONT_FACE


class FiveBallScene(Actor):
    def __init__(self):
        Actor.__init__(self)
        self.glut_sphere = GlutSphereActor()
        self.color = [0.2, 0.2, 0.5]
        self.sphere_imposter = SphereImposter(color=self.color)
        sphere = Vec3([0,0,0])
        sphere.center = Vec3([0,0,0])
        sphere.radius = 1.0
        sphere.color = self.color
        # self.sphere_array = VertexArrayTest()
        self.sphere_array = SphereImposterArray([sphere,])
        
    def init_gl(self):
        self.sphere_imposter.init_gl()
        self.sphere_array.init_gl()
        
    def paint_gl(self):
        c = self.color
        glColor3f(c[0], c[1], c[2]) # paint spheres blue
        glPushMatrix()
        glTranslate(-3.0, 0, 0)
        for p in range(5):
            # Leave a spot for shader sphere
            if 3 != p:
                self.glut_sphere.paint_gl()
            glTranslate(1.5, 0, 0)
        glPopMatrix()
        # TODO put shader sphere here
        # glColor3f(0.8, 0.8, 0.2) # yellow
        glPushMatrix()
        glTranslate(1.5, 0, 0)
        self.sphere_imposter.paint_gl()
        glTranslate(3.0, 0, 0)
        self.sphere_array.paint_gl()
        glPopMatrix()

