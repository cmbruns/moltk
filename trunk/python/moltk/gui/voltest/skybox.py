
import shader
from rotation import Rotation
from PySide.QtGui import QImage
from PySide.QtOpenGL import QGLWidget
from OpenGL.GL import *
import numpy
from math import sqrt

class SkyBoxShaderProgram(shader.ShaderProgram):
    def __init__(self):
        shader.ShaderProgram.__init__(self)
        self.vertex_shader = """
varying vec4 position;
uniform samplerCube skybox;
uniform float eye_shift;
void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec4 eye_position_in_camera = vec4(-eye_shift, 0, 0, 0); // compensate for parallax asymmetric frustum shift for stereo 3D
    position = gl_ModelViewMatrix * gl_Vertex - eye_position_in_camera;
    position.y *= -1; // put sun at top
    position.x *= -1; // keep coordinate frame right handed
}
"""
        self.fragment_shader = """
varying vec4 position;
uniform samplerCube skybox;
uniform float eye_shift;
void main()
{
    vec4 box_color = textureCube(skybox, position.xyz);
    gl_FragColor = box_color;
}
"""

    def __enter__(self):
        shader.ShaderProgram.__enter__(self)
        self.skybox = glGetUniformLocation(self.shader_program, "skybox")
        self.eye_shift = glGetUniformLocation(self.shader_program, "eye_shift")


def fname_to_tex(file_name):
        img = QImage()
        img.load(file_name)
        img2 = QGLWidget.convertToGLFormat(img)
        img3 = str(img2.bits())
        return img3

class SkyBox:
    def __init__(self):
        self.is_initialized = False
        self.shader = SkyBoxShaderProgram()

    def init_gl(self):
        if self.is_initialized:
            return
        self.texture_id = glGenTextures(1)
        glEnable(GL_TEXTURE_CUBE_MAP)
        glBindTexture(GL_TEXTURE_CUBE_MAP, self.texture_id)
        # Define all 6 faces
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE)
        if True:
            img = QImage()
            img.load("skybox/miramar_lf.tif")
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, img.width(), img.height(), 
                         0, GL_RGBA, GL_UNSIGNED_BYTE, fname_to_tex("skybox/miramar_ft.tif"))
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, img.width(), img.height(), 
                         0, GL_RGBA, GL_UNSIGNED_BYTE, fname_to_tex("skybox/miramar_bk.tif"))
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, img.width(), img.height(), 
                         0, GL_RGBA, GL_UNSIGNED_BYTE, fname_to_tex("skybox/miramar_dn.tif"))
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, img.width(), img.height(), 
                         0, GL_RGBA, GL_UNSIGNED_BYTE, fname_to_tex("skybox/miramar_up.tif"))
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, img.width(), img.height(), 
                         0, GL_RGBA, GL_UNSIGNED_BYTE, fname_to_tex("skybox/miramar_rt.tif"))
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, img.width(), img.height(), 
                         0, GL_RGBA, GL_UNSIGNED_BYTE, fname_to_tex("skybox/miramar_lf.tif"))
        else:
            test_img = numpy.array(256 * [50,50,128,255], 'uint8')
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, test_img)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, test_img)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, test_img)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, test_img)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, test_img)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA8, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, test_img)
        glDisable(GL_TEXTURE_CUBE_MAP)
        self.shader.init_gl()
        self.is_initialized = True
        
    def paint_gl(self, camera):
        if not self.is_initialized:
            self.init_gl()
        # print "painting skybox"
        glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT | GL_DEPTH_BUFFER_BIT)
        glDisable(GL_DEPTH_TEST)
        glDepthMask(False)
        glEnable(GL_TEXTURE_CUBE_MAP)
        glBindTexture(GL_TEXTURE_CUBE_MAP, self.texture_id)       
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR) 
        # Construct a giant cube, almost as big as possible without clipping
        max_coord = 1.0 / sqrt(2.0) * 0.5 * camera.zFar
        m = max_coord
        with self.shader:
            glUniform1i(self.shader.skybox, 0)
            glUniform1f(self.shader.eye_shift, camera.eye_shift_in_ground)
            glBegin(GL_QUADS)
            # front
            glVertex3f( m,  m, -m)
            glVertex3f(-m,  m, -m)
            glVertex3f(-m, -m, -m)
            glVertex3f( m, -m, -m)
            # back
            glVertex3f( m,  m,  m)
            glVertex3f( m, -m,  m)
            glVertex3f(-m, -m,  m)
            glVertex3f(-m,  m,  m)
            # right
            glVertex3f( m,  m,  m)
            glVertex3f( m,  m, -m)
            glVertex3f( m, -m, -m)
            glVertex3f( m, -m,  m)
            # left
            glVertex3f(-m,  m,  m)
            glVertex3f(-m, -m,  m)
            glVertex3f(-m, -m, -m)
            glVertex3f(-m,  m, -m)
            # down
            glVertex3f( m, -m,  m)
            glVertex3f( m, -m, -m)
            glVertex3f(-m, -m, -m)
            glVertex3f(-m, -m,  m)
            # up
            glVertex3f( m,  m,  m)
            glVertex3f(-m,  m,  m)
            glVertex3f(-m,  m, -m)
            glVertex3f( m,  m, -m)
            #
            glEnd()
        glPopAttrib()
