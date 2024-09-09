#TODO : Objekt Manager um Texturen, Wavefront Dateien(.obj) und Inhalte zu Laden und es in das VBO und VAO Programm einzutragen
from vbo import OBJVBO
from model import * 
#Lukas 9.9 --- Needs to be done
class ObjectManager:
    def __init__(self,app,scene):
        self.vbo = app.mesh.vao.vbo
        self.vao = app.mesh.vao
        self.vbos = self.vbo.vbos
        self.scene = scene
        self.texture = app.mesh.texture
        self.app = app
        self.objects = [["objww","objects/Eiffel Tower/UV Eiffel Tower.png"]]
    def load_Object(self,name,texture,data,pos=(0, -1, -10)):
        if name in self.vbos:
            return
        self.texture.textures[name] = self.texture.get_texture(path=texture)
        self.scene.add(OBJWavefront(self.app, pos=pos))
    def pass_VB(self,name : str):
        if name in self.vbos:
            return
        self.vbos[name] = OBJVBO(self.ctx)
        self.vao.vaos[name] = self.get_vao(
            program=self.vao.program.programs['default'],
            vbo=self.vao.vbo.vbos[name])
        # shadow vao
        self.vao.vaos['shadow_'+ name] = self.get_vao(
            program=self.vao.program.programs['shadow_map'],
            vbo=self.vao.vbo.vbos[name])