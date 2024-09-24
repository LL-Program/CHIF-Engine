import numpy as np
import moderngl as mgl
import pywavefront

class VBO:
    def __init__(self, ctx):
        self.vbos = {}
        self.ctx = ctx
        self.vbos['cube'] = CubeVBO(ctx)
        self.add_obj('objw', 'objects/Astronaut/astronaut.obj')
        self.vbos['skybox'] = SkyBoxVBO(ctx)
        self.vbos['advanced_skybox'] = AdvancedSkyBoxVBO(ctx)
        self.vbos['terrain'] = TerrainVBO(ctx)
    def destroy(self):
        for vbo in self.vbos.values():
            vbo.destroy()

    def add_obj(self, name, object):
        self.vbos[name] = OBJVBO(self.ctx, object)
class BaseVBO:
    def __init__(self, ctx, object=None):
        self.ctx = ctx
        self.object = object
        self.vbo = self.get_vbo()
        self.format: str = None
        self.attribs: list = None
    def get_vertex_data(self):
        raise NotImplementedError
    def get_vbo(self):
        vertex_data = self.get_vertex_data()
        return self.ctx.buffer(vertex_data)
    def destroy(self):
        self.vbo.release()
class OBJVBO(BaseVBO):
    def __init__(self, ctx, object):
        super().__init__(ctx, object)
        self.format = '2f 3f 3f'
        self.attribs = ['in_texcoord_0', 'in_normal', 'in_position']
    def get_vertex_data(self):
        objs = pywavefront.Wavefront(self.object, cache=True, parse=True)
        print("Loaded materials:", objs.materials)
        vertex_data = []
        for name, material in objs.materials.items():
            print(f"Material: {name} - Vertices count: {len(material.vertices)}")
            vertex_data.extend(material.vertices)
        return np.array(vertex_data, dtype='f4')
class TerrainVBO(BaseVBO):
    def __init__(self, ctx):
        super().__init__(ctx)
        self.format = '3f 3f 2f'
        self.attribs = ['in_position', 'in_normal', 'in_texcoord_0']

    def generate_height(self, x, z):
        return np.sin(x * 0.5) * np.cos(z * 0.5) * 2.0
    def get_vertex_data(self):
        grid_size = 50
        vertices = []
        normals = []
        texcoords = []
        indices = []
        for x in range(grid_size):
            for z in range(grid_size):
                height = self.generate_height(x, z)
                vertices.append((x, height, z))
                normals.append((0, 1, 0)) 
                texcoords.append((x / grid_size, z / grid_size))
        for x in range(grid_size - 1):
            for z in range(grid_size - 1):
                indices.extend([
                    (x * grid_size + z, (x + 1) * grid_size + z, x * grid_size + (z + 1)),
                    ((x + 1) * grid_size + z, (x + 1) * grid_size + (z + 1), x * grid_size + (z + 1))
                ])
        vertex_data = np.array([vertices[ind] for triangle in indices for ind in triangle], dtype='f4')
        normal_data = np.array([normals[ind] for triangle in indices for ind in triangle], dtype='f4')
        texcoord_data = np.array([texcoords[ind] for triangle in indices for ind in triangle], dtype='f4')
        vertex_data = np.hstack([vertex_data, normal_data, texcoord_data])
        return vertex_data


class CubeVBO(BaseVBO):
    def __init__(self, ctx):
        super().__init__(ctx)
        self.format = '2f 3f 3f'
        self.attribs = ['in_texcoord_0', 'in_normal', 'in_position']

    @staticmethod
    def get_data(vertices, indices):
        data = np.array([vertices[ind] for triangle in indices for ind in triangle], dtype='f4')
        return data

    def get_vertex_data(self):
        vertices = [(-1, -1, 1), (1, -1, 1), (1, 1, 1), (-1, 1, 1),
                    (-1, 1, -1), (-1, -1, -1), (1, -1, -1), (1, 1, -1)]

        indices = [(0, 2, 3), (0, 1, 2),
                   (1, 7, 2), (1, 6, 7),
                   (6, 5, 4), (4, 7, 6),
                   (3, 4, 5), (3, 5, 0),
                   (3, 7, 4), (3, 2, 7),
                   (0, 6, 1), (0, 5, 6)]
        vertex_data = self.get_data(vertices, indices)

        tex_coord_vertices = [(0, 0), (1, 0), (1, 1), (0, 1)]
        tex_coord_indices = [(0, 2, 3), (0, 1, 2),
                             (0, 2, 3), (0, 1, 2),
                             (0, 1, 2), (2, 3, 0),
                             (2, 3, 0), (2, 0, 1),
                             (0, 2, 3), (0, 1, 2),
                             (3, 1, 2), (3, 0, 1)]
        tex_coord_data = self.get_data(tex_coord_vertices, tex_coord_indices)

        normals = np.array([(0, 0, 1)] * 6 + [(1, 0, 0)] * 6 +
                           [(0, 0, -1)] * 6 + [(-1, 0, 0)] * 6 +
                           [(0, 1, 0)] * 6 + [(0, -1, 0)] * 6, dtype='f4').reshape(36, 3)

        vertex_data = np.hstack([normals, vertex_data])
        vertex_data = np.hstack([tex_coord_data, vertex_data])
        return vertex_data


class SkyBoxVBO(BaseVBO):
    def __init__(self, ctx):
        super().__init__(ctx)
        self.format = '3f'
        self.attribs = ['in_position']

    @staticmethod
    def get_data(vertices, indices):
        return np.array([vertices[ind] for triangle in indices for ind in triangle], dtype='f4')

    def get_vertex_data(self):
        vertices = [(-1, -1, 1), (1, -1, 1), (1, 1, 1), (-1, 1, 1),
                    (-1, 1, -1), (-1, -1, -1), (1, -1, -1), (1, 1, -1)]
        indices = [(0, 2, 3), (0, 1, 2),
                   (1, 7, 2), (1, 6, 7),
                   (6, 5, 4), (4, 7, 6),
                   (3, 4, 5), (3, 5, 0),
                   (3, 7, 4), (3, 2, 7),
                   (0, 6, 1), (0, 5, 6)]
        vertex_data = self.get_data(vertices, indices)
        vertex_data = np.flip(vertex_data, 1).copy(order='C')
        return vertex_data


class AdvancedSkyBoxVBO(BaseVBO):
    def __init__(self, ctx):
        super().__init__(ctx)
        self.format = '3f'
        self.attribs = ['in_position']

    def get_vertex_data(self):
        z = 0.9999
        vertices = [(-1, -1, z), (3, -1, z), (-1, 3, z)]
        return np.array(vertices, dtype='f4')
