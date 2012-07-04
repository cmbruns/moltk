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
    "Base class for shader programs."
    def __init__(self):
        self.previous_program = 0
        self.shader_program = 0
        self.vertex_shader = """
void main()
{
    // pass through shader
    gl_FrontColor = gl_Color;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}       
"""
        self.fragment_shader = """
void main()
{
    // pass through shader
    gl_FragColor = gl_Color;
}
"""

    def init_gl(self):  
        # print "creating shaders"
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

    def __enter__(self):
        self.previous_program = glGetIntegerv(GL_CURRENT_PROGRAM)
        try:
            glUseProgram(self.shader_program)   
        except OpenGL.error.GLError:
            print glGetProgramInfoLog(self.shader_program)
            raise
        
    def __exit__(self, type, value, tb):
        glUseProgram(self.previous_program)


class GreenShaderProgram(ShaderProgram):
    def __init__(self):
        ShaderProgram.__init__(self)
        self.fragment_shader = """
void main()
{
    gl_FragColor = vec4(0, 0.8, 0, 1);
}
"""


class SphereImposterShaderProgram(ShaderProgram):
    def __init__(self):
        ShaderProgram.__init__(self)
        self.vertex_shader = """
varying vec2 face_position;
void main()
{
    // We encode the quad corner offsets from the center in the vertex normal
    // Keep the quad oriented toward the screen
    vec4 corner_offset = gl_ProjectionMatrix * vec4(gl_Normal, 0);
    face_position = gl_Normal.xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex + corner_offset;
}
"""
        self.fragment_shader = """
varying vec2 face_position;
void main()
{
    if (length(face_position) > 1.0)
        discard;
    gl_FragColor = vec4(0.8, 0, 0, 1);
}
"""


class SphereImposter(Actor):
    "SphereImposter is still just a stub.  Eventually it will draw as sphere using shaders"
    def __init__(self):
        Actor.__init__(self)
        # self.shader_program = GreenShaderProgram()
        # self.shader_program = ShaderProgram()
        self.shader_program = SphereImposterShaderProgram()

    def init_gl(self):
        self.shader_program.init_gl()
        pass

    def paint_gl(self):
        with self.shader_program:
            glBegin(GL_TRIANGLE_STRIP)
            glNormal3f(-1, -1, 0)
            glVertex3f(0,0,0)
            glNormal3f( 1, -1, 0)
            glVertex3f(0,0,0)
            glNormal3f(-1,  1, 0)
            glVertex3f(0,0,0)
            glNormal3f( 1,  1, 0)
            glVertex3f(0,0,0)
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

