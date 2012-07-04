from OpenGL.GL import *
from OpenGL.GLUT import *


class Actor(object):
    def paint_gl(self):
        pass

    def init_gl(self):
        pass


class GlutSphereActor(Actor):
    def paint_gl(self):
        glutSolidSphere(1.0, 20, 20)


class TeapotActor(Actor):
    def paint_gl(self):
        glPushAttrib(GL_POLYGON_BIT) # remember current GL_FRONT_FACE indictor
        glFrontFace(GL_CW) # teapot polygon vertex order is opposite to modern convention
        glColor3f(0.3, 0.3, 0.3) # paint it gray
        glutSolidTeapot(1.0) # thank you GLUT tool kit
        glPopAttrib() # restore GL_FRONT_FACE


class SphereImposter(Actor):
    def paint_gl(self):
        glBegin(GL_TRIANGLE_STRIP)
        glVertex3f(-1,  1, 0)
        glVertex3f(-1, -1, 0)
        glVertex3f( 1, -1, 0)
        # glVertex3f( 1,  1, 0)
        glEnd()


class FiveBallScene(Actor):
    def __init__(self):
        Actor.__init__(self)
        self.glut_sphere = GlutSphereActor()
        self.sphere_imposter = SphereImposter()
        
    def paint_gl(self):
            glColor3f(0.2, 0.2, 0.5) # paint spheres blue
            glPushMatrix()
            glTranslate(-4.0, 0, 0)
            for p in range(5):
                # Leave a spot for shader sphere
                if 3 != p:
                    self.glut_sphere.paint_gl()
                glTranslate(2.0, 0, 0)
            glPopMatrix()
            # TODO put shader sphere here
            glColor3f(0.8, 0.8, 0.2) # yellow
            glPushMatrix()
            glTranslate(2.0, 0, 0)
            self.sphere_imposter.paint_gl()
            glPopMatrix()

