from Objects.vbo import VBO
from Render.shader_program import ShaderProgram

class VAO:
    def __init__(self, ctx):
        self.ctx = ctx
        self.vbo = VBO(ctx)
        self.program = ShaderProgram(ctx)
        self.vaos = {}
        self.create_vaos()
    def create_vaos(self):
        self.vaos['cube'] = self.get_vao(self.program.programs['default'], self.vbo.vbos['cube'])
        self.vaos['shadow_cube'] = self.get_vao(self.program.programs['shadow_map'], self.vbo.vbos['cube'])
        self.add_vao('objw')
        self.vaos['skybox'] = self.get_vao(self.program.programs['skybox'], self.vbo.vbos['skybox'])
        self.vaos['advanced_skybox'] = self.get_vao(self.program.programs['advanced_skybox'], self.vbo.vbos['advanced_skybox'])
    def add_vao(self, name):
        self.vaos[name] = self.get_vao(self.program.programs['default'], self.vbo.vbos[name])
        self.vaos['shadow_' + name] = self.get_vao(self.program.programs['shadow_map'], self.vbo.vbos[name])
    def get_vao(self, program, vbo):
        try:
            vao = self.ctx.vertex_array(program, [(vbo.vbo, vbo.format, *vbo.attribs)], skip_errors=True)
        except Exception as e:
            print(f"Error creating VAO: {e}")
            vao = None
        return vao
    def destroy(self):
        for vao in self.vaos.values():
            if vao:
                vao.release()
        self.vbo.destroy()
        self.program.destroy()
