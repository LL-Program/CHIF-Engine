#TODO : Objekt Manager um Texturen, Wavefront Dateien(.obj) und Inhalte zu Laden und es in das VBO und VAO Programm einzutragen
from Objects.vbo import VBO
from model import OBJWavefront
import pywavefront
import numpy as np

class ObjectManager:
    def __init__(self, app, scene):
        self.app = app
        self.scene = scene
        self.vao = app.mesh.vao
        self.texture = app.mesh.texture
        self.objects = {}  # Dictionary to store loaded objects
    def load_Object(self, name, obj_path, texture_path, pos=(0, -1, -10), scale=(1, 1, 1)):
        if name in self.objects:
            return
        obj_data = pywavefront.Wavefront(obj_path, create_materials=True, collect_faces=True)
        vertices = []
        for material_name, material in obj_data.materials.items():
            vertices.extend(material.vertices)
        vertices = np.array(vertices, dtype='f4')
        # Create VBO instance
        vbo = self.vao.vbo
        vbo.vbo = self.app.ctx.buffer(data=vertices)
        vbo.format = '3f 3f 2f'  # Assuming position, normal, and texture coordinates
        vbo.attribs = ['in_position', 'in_normal', 'in_texcoord_0']
        vbo_name = f"obj_{name}"
        self.vao.vbo.vbos[vbo_name] = vbo
        # Create VAOs for the object
        self.vao.vaos[vbo_name] = self.vao.get_vao(
            self.vao.program.programs['default'],
            vbo
        )
        self.vao.vaos[f"shadow_{vbo_name}"] = self.vao.get_vao(
            self.vao.program.programs['shadow_map'],
            vbo
        )
        # Load texture
        texture = self.texture.get_texture(texture_path)
        # Create OBJWavefront instance
        obj = OBJWavefront(self.app, vao_name=vbo_name, tex_id=texture, pos=pos, scale=scale)
        obj.collision_radius = max(scale) / 2  # Set collision radius
        # Store the object
        self.objects[name] = obj
        # Add the object to the scene
        return obj
    def get_object(self, name):
        return self.objects.get(name)
    def remove_object(self, name):
        if name in self.objects:
            obj = self.objects.pop(name)
            self.scene.remove(obj)
            # Clean up VAO and VBO
            vbo_name = f"obj_{name}"
            if vbo_name in self.vao.vaos:
                del self.vao.vaos[vbo_name]
            if f"shadow_{vbo_name}" in self.vao.vaos:
                del self.vao.vaos[f"shadow_{vbo_name}"]
            if vbo_name in self.vao.vbo.vbos:
                self.vao.vbo.vbos[vbo_name].destroy()
                del self.vao.vbo.vbos[vbo_name]