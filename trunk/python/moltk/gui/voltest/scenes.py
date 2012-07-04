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


class ShaderProgram:
    def __init__(self):
        self.vertex_shader = """
void main()
{
    // gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_Position = ftransform();
}        
"""
        self.fragment_shader = """
void main()
{
    gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}
"""

    def init_gl(self):  
        print "creating shaders"
        self.vs = glCreateShader(GL_VERTEX_SHADER)
        self.fs = glCreateShader(GL_FRAGMENT_SHADER)
        glShaderSource(self.vs, self.vertex_shader)
        glShaderSource(self.fs, self.fragment_shader)
        glCompileShader(self.vs)
        log = glGetShaderInfoLog(self.vs)
        if log:
            print "Vertex Shader:", log
        glCompileShader(self.fs)
        log = glGetShaderInfoLog(self.fs)
        if log:
            print "Fragment Shader:", log
        self.shader_program = glCreateProgram()
        glAttachShader(self.shader_program, self.vs)
        glAttachShader(self.shader_program, self.fs)
        glLinkProgram(self.shader_program)

    def use(self):
        glUseProgram(self.shader_program)
        
    def end(self):
        glUseProgram(0)

    def __enter__(self):
        self.use()
        
    def __exit__(self, type, value, tb):
        self.end()


class GreenShaderProgram(ShaderProgram):
    def __init__(self):
        ShaderProgram.__init__(self)
        self.vertex_shader = """
void main()
{
    gl_Position = ftransform();
}        
"""
        self.fragment_shader = """
void main()
{
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
"""


class SphereImposter(Actor):
    def __init__(self):
        Actor.__init__(self)
        self.shader_program = GreenShaderProgram()

    def init_gl(self):
        self.shader_program.init_gl()
        pass
        
    def paint_gl(self):
        with self.shader_program:
            glBegin(GL_TRIANGLE_STRIP)
            glVertex3f(-1, -1, 0)
            glVertex3f( 1, -1, 0)
            glVertex3f(-1,  1, 0)
            # glVertex3f( 1,  1, 0)
            glEnd()


class FiveBallScene(Actor):
    def __init__(self):
        Actor.__init__(self)
        self.glut_sphere = GlutSphereActor()
        self.sphere_imposter = SphereImposter()
        
    def init_gl(self):
        self.sphere_imposter.init_gl()
        
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

