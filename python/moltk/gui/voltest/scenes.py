from actor import Actor
from imposter import SphereImposterArray, CylinderImposterArray
from rotation import Vec3
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

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


class GlutCylinderActor(Actor):
    def __init__(self, position=[0,0,0], radius=1.0, height=2.0):
        self.position = position[:]
        self.radius = radius
        self.height = height

    def init_gl(self):
        self.quadric = gluNewQuadric()
        gluQuadricNormals(self.quadric, GLU_SMOOTH)
        gluQuadricTexture(self.quadric, GL_TRUE)
        
    def paint_gl(self):
        glMatrixMode(GL_MODELVIEW)
        glPushMatrix()
        p = self.position
        glTranslate(p[0], p[1], p[2] - self.height/2.0)
        gluCylinder(self.quadric, self.radius, self.radius, self.height, 20, 20)
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
        self.glut_cylinder = GlutCylinderActor()
        self.color = [0.2, 0.2, 0.5]
        sphere = Vec3([0,0,0])
        sphere.center = Vec3([0,0,0])
        sphere.radius = 1.0
        sphere.color = self.color
        # self.sphere_array = VertexArrayTest()
        self.sphere_array = SphereImposterArray([sphere,])
        cylinder = Vec3([0,0,0])
        cylinder.center = Vec3([0,0,0])
        cylinder.radius = 1.0
        cylinder.height = 2.0
        self.cylinder_array = CylinderImposterArray([cylinder,])
        
    def init_gl(self):
        self.sphere_array.init_gl()
        self.glut_cylinder.init_gl()
        self.cylinder_array.init_gl()
        
    def paint_gl(self):
        c = self.color
        glColor3f(c[0], c[1], c[2]) # paint spheres blue
        glPushMatrix()
        glTranslate(-3.0, 0, 0)
        for p in range(5):
            # Leave a spot for shader sphere
            if 3 == p:
                self.sphere_array.paint_gl()
            else:
                self.glut_sphere.paint_gl()
            glTranslate(1.5, 0, 0)
        glPopMatrix()
        glPushMatrix()
        glTranslate(-3.0, 2.0, 0)
        for p in range(5):
            # TODO Leave a spot for shader cylinder
            if 3 == p:
                self.cylinder_array.paint_gl()
            else:
                self.glut_cylinder.paint_gl()
            glTranslate(1.5, 0, 0)
        glPopMatrix()
        glPushMatrix()
        glTranslate(-3.0, 4.0, 0)
        for p in range(5):
            # TODO Leave a spot for shader cylinder
            self.glut_cylinder.paint_gl()
            glTranslate(1.5, 0, 0)
        glPopMatrix()
