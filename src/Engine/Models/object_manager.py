from Models.model import *

class ObjectManager:
    def __init__(self,engine):
        self.engine = engine
        self.active_Objects = {}
        self.mesh = engine.mesh
        self.texture = engine.mesh.texture
        self.vao = engine.mesh.vao
        self.vbo = engine.mesh.vao.vbo
        self.scene = engine.scene
        self.load_object('astronaut','objects/Astronaut/astronaut.obj','objects/Astronaut/astronaut.png')
    def load_object(self,name,object,texture,pos= (0,0,0),scale = (1,1,1),rot=(0,0,0)):
        self.vbo.add_obj(name,object)
        self.vao.add_vao(name)
        self.texture.add_texture(name,texture)
        self.scene.add_object(OBJWavefront(self.engine, vao_name=name, tex_id=name,rot=rot,pos=pos, scale=scale))
    def destroy_objects(self):
        pass