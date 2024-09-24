import glm
import moderngl
import numpy as np
class Gizmo:
    def __init__(self, app):
        self.app = app
        self.ctx = app.ctx
        self.program = self.create_shader_program()
        self.create_axes()
        self.selected_axis = None

    def create_shader_program(self):
        vertex_shader = '''
        #version 330
        uniform mat4 m_proj;
        uniform mat4 m_view;
        uniform mat4 m_model;
        in vec3 in_position;
        in vec3 in_color;
        out vec3 v_color;
        void main() {
            gl_Position = m_proj * m_view * m_model * vec4(in_position, 1.0);
            v_color = in_color;
        }
        '''
        fragment_shader = '''
        #version 330
        in vec3 v_color;
        out vec4 frag_color;
        void main() {
            frag_color = vec4(v_color, 1.0);
        }
        '''
        return self.ctx.program(vertex_shader=vertex_shader, fragment_shader=fragment_shader)

    def create_axes(self):
        self.vertices = [
            (-1, 0, 0), (1, 0, 0),  
            (0, -1, 0), (0, 1, 0),  
            (0, 0, -1), (0, 0, 1)  
        ]
        self.colors = [
            (1, 0, 0), (1, 0, 0),   
            (0, 1, 0), (0, 1, 0),   
            (0, 0, 1), (0, 0, 1)    
        ]
        self.vbo = self.ctx.buffer(data=np.array(self.vertices + self.colors, dtype='f4').tobytes())
        self.vao = self.ctx.simple_vertex_array(self.program, self.vbo, 'in_position', 'in_color')

    def render(self, model_matrix):
        self.program['m_proj'].write(self.app.camera.m_proj)
        self.program['m_view'].write(self.app.camera.m_view)
        self.program['m_model'].write(model_matrix)
        self.vao.render(moderngl.LINES)
