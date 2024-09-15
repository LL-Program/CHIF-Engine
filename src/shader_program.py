import os

class ShaderProgram:
    def __init__(self, ctx):
        self.ctx = ctx
        self.programs = {}
        self.programs['default'] = self.get_program('default')
        self.programs['skybox'] = self.get_program('skybox')
        self.programs['advanced_skybox'] = self.get_program('advanced_skybox')
        self.programs['shadow_map'] = self.get_program('shadow_map')

    def get_program(self, shader_program_name):
        shader_dir = os.path.join(os.path.dirname(__file__), 'shaders')
        vertex_shader_path = os.path.join(shader_dir, f'{shader_program_name}.vert')
        fragment_shader_path = os.path.join(shader_dir, f'{shader_program_name}.frag')
        with open(vertex_shader_path) as file:
            vertex_shader = file.read()
        with open(fragment_shader_path) as file:
            fragment_shader = file.read()
        program = self.ctx.program(vertex_shader=vertex_shader, fragment_shader=fragment_shader)
        return program

    def destroy(self):
        [program.release() for program in self.programs.values()]
