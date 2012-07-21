from shader import ShaderProgram
import shader
from OpenGL.GL import *
import numpy
from math import pi, cos, sin


class SphereImposterShaderProgram(ShaderProgram):
    def __enter__(self):
        ShaderProgram.__enter__(self)
        # print self.zNear, type(self.zNear), self.shader_program, glGetUniformLocation(self.shader_program, "zNear")
        glUniform1f(glGetUniformLocation(self.shader_program, "zNear"), self.zNear)
        glUniform1f(glGetUniformLocation(self.shader_program, "zFar"), self.zFar)
        glUniform1f(glGetUniformLocation(self.shader_program, "eye_shift"), self.eye_shift)
        bg = self.background_color
        glUniform4f(glGetUniformLocation(self.shader_program, "background_color"), 
                    bg[0], bg[1], bg[2], bg[3])
        return self
        
    def __init__(self):
        ShaderProgram.__init__(self)
        self.vertex_shader = """
#version 120
// TODO should be uniform
varying mat4 cameraToClipMatrix;
uniform float zNear;
uniform float zFar;
uniform vec4 background_color;
uniform float eye_shift; // for asymmetric frustum stereo

varying vec3 position_in_eye;
varying vec3 sphere_center_in_eye;
varying float tangent_distance_squared; // precompute for use in ray casting
varying float max_front_clip;
varying float radius_squared;
varying float radius;
varying float z_front; // positive distance from sphere center to front clip plane
const float near_clip_zone = 0.005;
const float max_radius = 2.0;

void main()
{
    // We store a quad as 4 points with the same vertex, but different normals.
    // Reconstruct the quad by adding the normal to the vertex.
    radius = gl_Normal.z;
    vec4 sc = gl_ModelViewMatrix * gl_Vertex;
    vec3 sphere_center_in_camera = sc.xyz * 1.0/sc.w;

    // Parry front and rear clip planes
    vec3 frontmost_in_camera = sphere_center_in_camera + vec3(0, 0, radius);
    vec3 rearmost_in_camera = sphere_center_in_camera - vec3(0, 0, radius);
    cameraToClipMatrix = gl_ProjectionMatrix;
    max_front_clip = (cameraToClipMatrix * vec4(frontmost_in_camera, 1)).z;
    // Actual clip plane for spheres, including near_clip_zone
    vec4 near2_clip = gl_ProjectionMatrix * vec4(0, 0, -zNear, 1);
    near2_clip.z = near_clip_zone;
    float zNear2 = -(gl_ProjectionMatrixInverse * near2_clip).z;
    z_front = -sphere_center_in_camera.z - zNear;
    float max_rear_clip = (cameraToClipMatrix * vec4(rearmost_in_camera, 1)).z;
    // translate imposter plane to optimize clipped appearance

    float relative_quad_position = radius; // default to billboard behind sphere, for best front clipping
    if ((max_rear_clip > 1.0) && (max_front_clip > 0.0))
        relative_quad_position = -radius; // move to front for nice simple rear clipping
    else if (max_rear_clip < 0.0) ; // vertex and sphere should fail front clip
    else if (max_front_clip > 1.0) ; // vertex and sphere should fail rear clip
    else if ((max_rear_clip > 1.0) && (max_front_clip < 0.0))
        // double clip! choose an intermediate distance for the quad
        relative_quad_position = 0.5*(zNear + zFar) + sphere_center_in_camera.z ; // move to front for nice simple rear clipping

    vec3 eye_position_in_camera = vec3(-eye_shift, 0, 0); // compensate for parallax asymmetric frustum shift for stereo 3D
    // vec3 eye_position_in_camera = vec3(0, 0, 0); // compensate for parallax asymmetric frustum shift for stereo 3D
    sphere_center_in_eye = sphere_center_in_camera - eye_position_in_camera;
    vec3 eye_direction = normalize(-sphere_center_in_eye);
    // push quad back to the far back of the sphere, to avoid near clipping
    vec3 quad_center_in_camera = sphere_center_in_camera - relative_quad_position * eye_direction;
    // orient quad perpendicular to camera direction (this rotates normal by about 90 degrees, but that's OK)
    float circumradius_length = length(vec2(gl_Normal.xy));
    vec3 corner_offset = normalize(cross(vec3(gl_Normal.xy,0), eye_direction));
    
    // scale the corner distance
    //  a) - by sqrt(2) because it's a corner: sqrt(1*1 + 1*1)
    //  b) - by radius because that's how big the sphere is
    //  c) - by (d+r)/d because of recessed quad position, 
    //  d) - by d/sqrt(d^2-r^2) for bulge of horizon tangent
    //  e) - by 1.05 because it's too small toward the edge of the screen for some reason
    float r = radius;
    float d2 = dot(sphere_center_in_eye, sphere_center_in_eye);
    float d = sqrt(d2);
    radius_squared = radius * radius;
    tangent_distance_squared = d2 - radius_squared;
    float corner_scale = 1.05 * circumradius_length * r*(d+relative_quad_position)/sqrt(tangent_distance_squared);
    corner_offset = corner_scale * corner_offset;
    // Keep the quad oriented toward the camera by not rotating corner_offset.
    vec3 position_in_camera = quad_center_in_camera + corner_offset;
    gl_Position = gl_ProjectionMatrix * vec4(position_in_camera, 1); // in clip
    
    // Put quantities in eye_frame not camera frame for fragment shader use.
    // This only matters in asymmetric frustum stereo 3D
    position_in_eye = position_in_camera - eye_position_in_camera;

    gl_FrontColor = gl_Color;
}
"""
        self.fragment_shader = """
#version 120
// TODO cameraToClipMatrix should be uniform
varying mat4 cameraToClipMatrix;
uniform vec4 background_color;
uniform float zNear;
varying float max_front_clip;
varying vec3 position_in_eye;
varying vec3 sphere_center_in_eye;
varying float tangent_distance_squared; // precompute for use in ray casting
varying float radius_squared;
varying float radius;
varying float z_front; // distance from sphere center to front clip plane
const float near_clip_zone = 0.005;
const float max_radius = 4.0;

void main()
{
    // Ray casting for actual surface point
    // Use quadratic equation - factor of 2 is elided
    float a = dot(position_in_eye, position_in_eye);
    float b = dot(position_in_eye, sphere_center_in_eye);
    float c = tangent_distance_squared;
    float det = b*b - a*c;
    if (det <= 0.0) {
        discard;  // No tangent? past sphere edge
        // for debugging...
        // gl_FragColor = vec4(1,0,0,1);
        // return;
    }
    float alpha1 = (b - sqrt(det))/a;
    float alpha2 = (b + sqrt(det))/a;
    vec3 surface_in_eye = alpha1 * position_in_eye.xyz;
    
    // cull by front and rear clip planes
    vec4 depth_vec = cameraToClipMatrix * vec4(surface_in_eye, 1);
    float depth = ((depth_vec.z / depth_vec.w) + 1.0) * 0.5;
    if (depth >= 1.0)
        discard; // front of sphere is behind back clip plane
    vec3 back_surface_in_eye = alpha2 * position_in_eye;
    depth_vec = cameraToClipMatrix * vec4(back_surface_in_eye, 1);
    float depth2 = ((depth_vec.z / depth_vec.w) + 1.0) * 0.5;
    if (depth2 <= 0.0)
        discard; // back of sphere is in front of front clip plane
    
    vec3 surface_in_sphere = surface_in_eye - sphere_center_in_eye;
    vec3 normal_in_camera = normalize(surface_in_sphere);
    
    // Correct depth for Z buffer
    // Use a finite near slice of the frustum to handle solid-core clip overlaps,
    // so two intersecting near-clipped spheres are segregated more gracefully.
    if (depth <= near_clip_zone) { // front clip bisects this sphere, show solid core
        // clip to reveal a solid core
        // tiny offset so neighboring clipped spheres overlap correctly
        float m = radius * (-zNear - sphere_center_in_eye.z - 0.20) / surface_in_sphere.z;
        float rel_depth = 1.0 - (radius*radius - m*m) / max_radius;
        rel_depth = clamp(rel_depth, 0.0, 1.0);
        gl_FragDepth = 0.0 + near_clip_zone * rel_depth; // overlap
        normal_in_camera = vec3(0, 0, 1); // slice it flat against screen
    }
    else {
        gl_FragDepth = depth;
    }
    
    // Use Lambertian shading model
    // DIFFUSE
    // from http://www.davidcornette.com/glsl/glsl.html
    // TODO - distinguish sources at infinity from local sources
    vec3 s = -normalize(surface_in_eye - gl_LightSource[0].position.xyz);
    vec3 lightvec_in_camera = s;
    vec3 n = normalize(normal_in_camera);
    vec3 r = normalize(-reflect(lightvec_in_camera, n));
    vec3 v = normalize(-surface_in_eye.xyz);
    // vec4 diffuse = gl_FrontMaterial.diffuse * max(0.0, dot(n, s)) * gl_LightSource[0].diffuse;
    vec4 diffuse = gl_Color * max(0.0, dot(n, s)) * gl_LightSource[0].diffuse;
    // SPECULAR
    vec4 specular = vec4(0,0,0,0);
    if (gl_FrontMaterial.shininess != 0.0)
        specular = gl_LightSource[0].specular * 
            gl_FrontMaterial.specular * 
            // TODO - Seems I have to divide by 5 to get the same effect as fixed pipeline
            // Perhaps I am not properly handling light source at infinity...
            pow(max(0.0, dot(r, v)), 0.20 * gl_FrontMaterial.shininess);
    // AMBIENT
    vec4 ambient =  gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
    // vec4 sceneColor = gl_FrontLightModelProduct.sceneColor;
    vec4 sceneColor = gl_FrontMaterial.emission + gl_Color * gl_LightModel.ambient;
    
    vec4 objectColor = sceneColor + ambient + diffuse + specular;

    // FOG
    float fog_start = 0.80;
    vec4 fogColor = background_color;
    float fog_ratio = 0.0;
    if (depth > fog_start)
        fog_ratio = (fog_start - depth) / (fog_start - 1.0);
    fog_ratio = clamp(fog_ratio, 0.0, 1.0);

    gl_FragColor = mix(objectColor, fogColor, fog_ratio);
}
"""

sphereImposterShaderProgram = SphereImposterShaderProgram()
        

cylinderImposterShaderProgram = shader.GreenShaderProgram()


class VertexArrayTest:
    def __init__(self):
        # self.vertex_array = numpy.array([1.0,1.0,0.0,1.0, -1.0,1.0,0.0,1.0, -1.0,-1.0,0.0,1.0], dtype='float32')
        self.vertex_array = numpy.array([0.0,0.0,0.0,1.0, 0.0,0.0,0.0,1.0, 0.0,0.0,0.0,1.0], dtype='float32')
        self.normal_array = numpy.array([1.0,1.0,1.0, -1.0,1.0,1.0, -1.0,-1.0,1.0], dtype='float32')
        self.color_array = numpy.array([0.0,1.0,0.0,1.0, 0.0,1.0,0.0,1.0, 0.0,1.0,0.0,1.0], dtype='float32')
        self.index_array = numpy.array([0, 1, 2], dtype='uint32')
        
    def init_gl(self):
        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT)
        # Vertices
        self.vertex_buffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.vertex_buffer)
        glBufferData(GL_ARRAY_BUFFER, self.vertex_array, GL_STATIC_DRAW)
        # Normals
        self.normal_buffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.normal_buffer)
        glBufferData(GL_ARRAY_BUFFER, self.normal_array, GL_STATIC_DRAW)
        # Colors
        self.color_buffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.color_buffer)
        glBufferData(GL_ARRAY_BUFFER, self.color_array, GL_STATIC_DRAW)
        # Vertex indices
        self.index_buffer = glGenBuffers(1)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self.index_buffer)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, self.index_array, GL_STATIC_DRAW)
        glPopClientAttrib()
        
    def paint_gl(self):
        with sphereImposterShaderProgram:
            glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT)
            # Vertices
            glBindBuffer(GL_ARRAY_BUFFER, self.vertex_buffer)
            glEnableClientState(GL_VERTEX_ARRAY)
            glVertexPointer(4, GL_FLOAT, 0, None)
            # Normals
            glBindBuffer(GL_ARRAY_BUFFER, self.normal_buffer)
            glEnableClientState(GL_NORMAL_ARRAY)
            glNormalPointer(GL_FLOAT, 0, None)
            # Colors
            glBindBuffer(GL_ARRAY_BUFFER, self.color_buffer)
            glEnableClientState(GL_COLOR_ARRAY)
            glColorPointer(4, GL_FLOAT, 0, None)
            # Vertex indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self.index_buffer)
            glColor3f(1,0,0)
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, None)
            # glDrawArrays(GL_TRIANGLES, 0, 3)
            glPopClientAttrib()
            

class ImposterQuadArray:
    def __init__(self):
        self.vertex_count = 4 # one quadrilateral per sphere
        self.triangle_count = self.vertex_count - 2
       
    
    
class SphereImposterArray:
    def __init__(self, spheres):
        self.vertex_count = 4 # one quadrilateral per sphere
        self.triangle_count = self.vertex_count - 2
        self.normal_offsets = []
        # Compute unit offsets from center of sphere to imposter polygon vertices
        # Scaled so apothem length is 1.0
        angle = 0.0
        d_angle = 2.0 * pi / self.vertex_count # to next vertex of polygon
        apothem_length = 1.0
        R = circumradius_length = apothem_length / cos(pi/self.vertex_count)
        for v in range(self.vertex_count):
            n = [R * cos(angle), R * sin(angle)]
            self.normal_offsets.append(n, )
            angle += d_angle
        self.vertex_array = numpy.array(list(self._vertex_generator(spheres)), dtype='float32')
        self.normal_array = numpy.array(list(self._normal_generator(spheres)), dtype='float32')
        self.color_array = numpy.array(list(self._color_generator(spheres)), dtype='float32')
        self.index_array = numpy.array(list(self._index_generator(spheres)), dtype='uint32')
        self.vertex_buffer = 0
        self.color_buffer = 0
        self.normal_buffer = 0
        self.index_buffer = 0
        self.is_initialized = False
        
    def __del__(self):
        glDeleteBuffers(1, self.vertex_buffer)
        glDeleteBuffers(1, self.color_buffer)
        glDeleteBuffers(1, self.index_buffer)
        glDeleteBuffers(1, self.normal_buffer)
        
    def init_gl(self):
        if self.is_initialized:
            return
        glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT)
        # Vertices
        self.vertex_buffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.vertex_buffer)
        glBufferData(GL_ARRAY_BUFFER, self.vertex_array, GL_STATIC_DRAW)
        # Normals
        self.normal_buffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.normal_buffer)
        glBufferData(GL_ARRAY_BUFFER, self.normal_array, GL_STATIC_DRAW)
        # Colors
        self.color_buffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.color_buffer)
        glBufferData(GL_ARRAY_BUFFER, self.color_array, GL_STATIC_DRAW)
        # Vertex indices
        self.index_buffer = glGenBuffers(1)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self.index_buffer)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, self.index_array, GL_STATIC_DRAW)
        glPopClientAttrib()
        self.is_initialized = True
                
    def paint_gl(self):
        with sphereImposterShaderProgram:
            if not self.is_initialized:
                self.init_gl()
            glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT)
            # Vertices
            glBindBuffer(GL_ARRAY_BUFFER, self.vertex_buffer)
            glEnableClientState(GL_VERTEX_ARRAY)
            glVertexPointer(4, GL_FLOAT, 0, None)
            # Normals
            glBindBuffer(GL_ARRAY_BUFFER, self.normal_buffer)
            glEnableClientState(GL_NORMAL_ARRAY)
            glNormalPointer(GL_FLOAT, 0, None)
            # Colors
            glBindBuffer(GL_ARRAY_BUFFER, self.color_buffer)
            glEnableClientState(GL_COLOR_ARRAY)
            glColorPointer(3, GL_FLOAT, 0, None)
            # Vertex indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self.index_buffer)
            glDrawElements(GL_TRIANGLES, len(self.vertex_array), GL_UNSIGNED_INT, None)
            glPopClientAttrib()
        
    def _vertex_generator(self, spheres):
        for sphere in spheres:
            p = sphere.center
            # Every vertex has the sphere center as its main coordinate
            for v in range(self.vertex_count):
                yield p[0]
                yield p[1]
                yield p[2]
                yield 1.0
            
    def _color_generator(self, spheres):
        for sphere in spheres:
            p = sphere.color
            for v in range(self.vertex_count):
                yield p[0]
                yield p[1]
                yield p[2]
                # yield 1.0
            
    def _normal_generator(self, spheres):
        for sphere in spheres:
            r = sphere.radius
            # normal contains unit offset from center in x,y, and radius in z
            for v in range(self.vertex_count):
                yield self.normal_offsets[v][0] # x offset from center
                yield self.normal_offsets[v][1] # y offset from center
                yield r # sphere radius
                
    def _index_generator(self, spheres):
        "triangle vertex indices"
        vertex_offset = 0
        for sphere in spheres:
            v0 = vertex_offset + 0 # index of first vertex in polygon
            vn = vertex_offset + self.vertex_count - 1 # index of final vertex in polygon
            for t in range(self.triangle_count):
                yield v0 + t
                yield v0 + t + 1
                yield vn
            vertex_offset += self.vertex_count


class CylinderImposterShaderProgram(ShaderProgram):
    pass # TODO


class CylinderImposterArray:
    def __init__(self, cylinders):
        self.cylinders = cylinders
    
    def init_gl(self):
        cylinderImposterShaderProgram.init_gl()

    def paint_gl(self):
        with cylinderImposterShaderProgram:
            for c in self.cylinders:
                glBegin(GL_TRIANGLES)
                glVertex3f(c.radius,0,-c.height/2.0)
                glVertex3f(-c.radius,0, c.height/2.0)
                glVertex3f(c.radius,0, c.height/2.0)
                glVertex3f(c.radius,0,-c.height/2.0)
                glVertex3f(-c.radius,0, -c.height/2.0)
                glVertex3f(-c.radius,0, c.height/2.0)
                # TODO
                glEnd()
   
