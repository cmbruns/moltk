
import shader
from rotation import Rotation
from PySide.QtGui import QImage
from PySide.QtOpenGL import QGLWidget
from OpenGL.GL import *
import numpy

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
    position = gl_Vertex;
    position.x += eye_shift;
}       
"""
        self.fragment_shader = """
varying vec4 position;
uniform samplerCube skybox;
uniform float eye_shift;
void main()
{
    vec3 tex_coord = -position.xyz;
    vec4 box_color = textureCube(skybox, tex_coord);
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
        # remove camera rotation
        old_rotation = camera.rotation
        camera.rotation = Rotation()
        with camera:
            z = 0.5 * (camera.zNear - camera.zFar)
            w = 10*camera.zFar;
            with self.shader:
                glUniform1i(self.shader.skybox, 0)
                glUniform1f(self.shader.eye_shift, camera.eye_shift_in_ground)
                glBegin(GL_QUADS)
                glVertex3d( w,  w, z)
                glVertex3d(-w,  w, z)
                glVertex3d(-w, -w, z)
                glVertex3d( w, -w, z)
                glEnd()
        camera.rotation = old_rotation
        glPopAttrib()
