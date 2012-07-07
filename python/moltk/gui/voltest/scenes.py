from OpenGL.GL import *
from OpenGL.GLUT import *


class Actor(object):
    def paint_gl(self):
        pass

    def init_gl(self):
        pass


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


class ShaderProgram:
    "Base class for shader programs."
    def __init__(self):
        self.is_initialized = False
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
        if self.is_initialized:
            return
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
        self.is_initialized = True

    def __enter__(self):
        self.previous_program = glGetIntegerv(GL_CURRENT_PROGRAM)
        try:
            glUseProgram(self.shader_program)
        except OpenGL.error.GLError:
            print glGetProgramInfoLog(self.shader_program)
            raise
        return self
        
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
    def __enter__(self):
        ShaderProgram.__enter__(self)
        glUniform1f(glGetUniformLocation(self.shader_program, "zNear"), self.zNear)
        return self
        
    def __init__(self):
        ShaderProgram.__init__(self)
        self.vertex_shader = """
// TODO should be uniform
varying mat4 cameraToClipMatrix;
uniform float zNear;

varying vec4 position_in_eye;
varying vec4 sphere_center_in_eye;
varying float tangent_distance_squared; // precompute for use in ray casting
varying float max_front_clip;
void main()
{
    // We store a quad as 4 points with the same vertex, but different normals.
    // Reconstruct the quad by adding the normal to the vertex.
    float radius = gl_Normal.z;
    sphere_center_in_eye = gl_ModelViewMatrix * gl_Vertex;
    vec4 eye_direction = normalize(vec4(-sphere_center_in_eye.xyz, 0));
    // push quad back to the far back of the sphere, to avoid near clipping
    vec4 quad_center_in_eye = sphere_center_in_eye - radius * eye_direction;
    // orient quad perpendicular to eye direction (this rotates normal by about 90 degrees, but that's OK)
    vec4 corner_offset = normalize(vec4(cross(vec3(gl_Normal.xy,0), eye_direction.xyz),0));
    // scale the corner distance
    //  a) - by sqrt(2) because it's a corner: sqrt(1*1 + 1*1)
    //  b) - by radius because that's how big the sphere is
    //  c) - by (d+r)/d because of recessed quad position, 
    //  d) - by d/sqrt(d^2-r^2) for bulge of horizon tangent
    //  e) - by 1.05 because it's too small toward the edge of the screen for some reason
    float r = radius;
    float d2 = dot(sphere_center_in_eye.xyz, sphere_center_in_eye.xyz);
    float d = sqrt(d2);
    tangent_distance_squared = d2 - radius*radius;
    float corner_scale = 1.05 * 1.41421356 * r*(d+r)/sqrt(tangent_distance_squared);
    corner_offset = corner_scale * corner_offset;
    // Keep the quad oriented toward the eye by not rotating corner_offset.
    position_in_eye = quad_center_in_eye + corner_offset;
    cameraToClipMatrix = gl_ProjectionMatrix;
    
    vec4 v_max_front_clip = cameraToClipMatrix * (sphere_center_in_eye + vec4(0, 0, radius, 0));
    max_front_clip = v_max_front_clip.z;       

    gl_Position = gl_ProjectionMatrix * position_in_eye; // in screen
}
"""
        self.fragment_shader = """
// TODO should be uniform
varying mat4 cameraToClipMatrix;
varying float max_front_clip;

varying vec4 position_in_eye;
varying vec4 sphere_center_in_eye;
varying float tangent_distance_squared; // precompute for use in ray casting
void main()
{
    // Ray casting for actual surface point
    // Use quadratic equation - factor of 2 is elided
    float a = dot(position_in_eye.xyz, position_in_eye.xyz);
    float b = dot(position_in_eye.xyz, sphere_center_in_eye.xyz);
    float c = tangent_distance_squared;
    float det = b*b - a*c;
    if (det <= 0.0)
        discard;  // No tangent? past sphere edge
    float alpha1 = (b - sqrt(det))/a;
    float alpha2 = (b + sqrt(det))/a;
    vec4 surface_in_eye = vec4(alpha1 * position_in_eye.xyz, 1);
    
    // cull depth
    vec4 depth_vec = cameraToClipMatrix * surface_in_eye;
    float depth = ((depth_vec.z / depth_vec.w) + 1.0) * 0.5;
    if (depth >= 1.0)
        discard; // front of sphere is behind back clip plane
    
    vec4 back_surface_in_eye = vec4(alpha2 * position_in_eye.xyz, 1);
    depth_vec = cameraToClipMatrix * back_surface_in_eye;
    float depth2 = ((depth_vec.z / depth_vec.w) + 1.0) * 0.5;
    if (depth2 <= 0.0)
        discard; // back of sphere is in front of front clip plane
    
    vec3 normal_in_eye = normalize(surface_in_eye.xyz - sphere_center_in_eye.xyz);
    
    // Correct depth for Z buffer
    if (depth <= 0.01) { // front clip bisects this sphere, show solid core
        // clip to reveal a solid core
        // tiny offset so neighboring clipped spheres overlap correctly
        float rel_depth = 1.0 - depth/max_front_clip;
        gl_FragDepth = 0.0 + 0.01 * rel_depth; // TODO overlap
        normal_in_eye = vec3(0, 0, 1); // slice it flat against screen
    }
    else {
        gl_FragDepth = depth;
    }
    
    // DIFFUSE
    // from http://www.davidcornette.com/glsl/glsl.html
    
    vec4 s = -normalize(surface_in_eye - gl_LightSource[0].position);
    vec3 lightvec_in_eye = s.xyz;
    
    vec3 n = normalize(normal_in_eye);
    vec3 r = normalize(-reflect(lightvec_in_eye, n));
    vec3 v = normalize(-surface_in_eye.xyz);
    vec4 diffuse = gl_FrontMaterial.diffuse * max(0.0, dot(n, s.xyz)) * gl_LightSource[0].diffuse;
    
    // Use Lambertian shading model
    vec4 specular = vec4(0,0,0,0);
    // SPECULAR
    if (gl_FrontMaterial.shininess != 0.0)
        specular = gl_LightSource[0].specular * 
            gl_FrontMaterial.specular * 
            // TODO - Seems I have to divide by 5 to get the same effect as fixed pipeline
            pow(max(0.0, dot(r, v)), 0.20 * gl_FrontMaterial.shininess);
    
    // AMBIENT
    vec4 ambient =  gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

    vec4 sceneColor = gl_FrontLightModelProduct.sceneColor;
    
    gl_FragColor = sceneColor + ambient + diffuse + specular;    
}
"""

sphereImposterShaderProgram = SphereImposterShaderProgram()

class SphereImposter(Actor):
    def __init__(self, location=[0.0, 0.0, 0.0], radius = 1.0, color=[0.2, 0.2, 0.3]):
        Actor.__init__(self)
        self.shader_program = sphereImposterShaderProgram
        self.location = location[:]
        self.radius = radius
        self.color = color[:]

    def paint_gl(self):
        glPushMatrix()
        c = self.color
        p = self.location
        glTranslate(p[0], p[1], p[2])
        radius = self.radius
        with self.shader_program:
            glColor3f(c[0], c[1], c[2])
            # two triangles
            glBegin(GL_TRIANGLES)
            #
            # Normals are not normals
            # Normal encodes quad corner direction in x,y, and radius in z
            glNormal3f(-1, -1, radius)
            glVertex3f(0,0,0)
            glNormal3f( 1, -1, radius)
            glVertex3f(0,0,0)
            glNormal3f(-1,  1, radius)
            glVertex3f(0,0,0)
            #
            glNormal3f( 1, -1, radius)
            glVertex3f(0,0,0)
            glNormal3f( 1,  1, radius)
            glVertex3f(0,0,0)
            glNormal3f(-1,  1, radius)
            glVertex3f(0,0,0)
            glEnd()
        glPopMatrix()

class FiveBallScene(Actor):
    def __init__(self):
        Actor.__init__(self)
        self.glut_sphere = GlutSphereActor()
        self.color = [0.2, 0.2, 0.5]
        self.sphere_imposter = SphereImposter(color=self.color)
        
    def init_gl(self):
        self.sphere_imposter.init_gl()
        
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
        glPopMatrix()

