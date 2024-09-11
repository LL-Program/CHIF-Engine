import moderngl as mgl
import numpy as np
import glm
from Logic.enginesettings import *

class BaseModel:
    def __init__(self, app, vao_name, tex_id, pos=(0, 0, 0), rot=(0, 0, 0), scale=(1, 1, 1)):
        self.app = app
        self.pos = glm.vec3(pos)
        self.rot = glm.vec3([glm.radians(a) for a in rot])
        self.scale = glm.vec3(scale)

        # New physics properties
        self.velocity = glm.vec3(0, 0, 0)  # Initial velocity
        self.acceleration = glm.vec3(0, 0, 0)  # Initial acceleration
        self.mass = 1.0  # Default mass
        self.gravity = glm.vec3(0, -0.0000003, 0)  # Gravity
        self.m_model = self.get_model_matrix()
        self.vao_name = vao_name
        self.tex_id = tex_id
        self.vao = app.mesh.vao.vaos[vao_name]
        self.program = self.vao.program
        self.camera = self.app.camera
    def update(self): pass
    def apply_force(self, force):
        """Apply an external force to the object"""
        self.acceleration += force / self.mass
    def update_physics(self, delta_time):
        if self.app.physics:
            """Update position based on velocity and acceleration"""
            self.apply_force(self.gravity * self.mass)  # Apply gravity force
            self.velocity += self.acceleration * delta_time
            self.pos += self.velocity * delta_time
            self.acceleration = glm.vec3(0, 0, 0)  # Reset acceleration after each frame
        else: pass
    def get_model_matrix(self):
        m_model = glm.mat4()
        m_model = glm.translate(m_model, self.pos)
        # rotate
        m_model = glm.rotate(m_model, self.rot.z, glm.vec3(0, 0, 1))
        m_model = glm.rotate(m_model, self.rot.y, glm.vec3(0, 1, 0))
        m_model = glm.rotate(m_model, self.rot.x, glm.vec3(1, 0, 0))
        # scale
        m_model = glm.scale(m_model, self.scale)
        return m_model
    def get_aabb(self):
        """Returns the Axis-Aligned Bounding Box (AABB) of the object."""
        half_scale = self.scale * 0.5
        min_bounds = self.pos - half_scale
        max_bounds = self.pos + half_scale
        return min_bounds, max_bounds

    def render(self):
        self.update()
        self.vao.render()
class ExtendedBaseModel(BaseModel):
    def __init__(self, app, vao_name, tex_id, pos, rot, scale):
        super().__init__(app, vao_name, tex_id, pos, rot, scale)
        self.on_init()
    def update(self):
        self.texture.use(location=0)
        self.program['camPos'].write(self.camera.position)
        self.program['m_view'].write(self.camera.m_view)
        self.program['m_model'].write(self.m_model)
        self.update_physics(self.app.delta_time)
        self.m_model = self.get_model_matrix()
    def update_shadow(self):
        self.shadow_program['m_model'].write(self.m_model)
    def render_shadow(self):
        self.update_shadow()
        self.shadow_vao.render()
    def on_init(self):
        self.program['m_view_light'].write(self.app.light.m_view_light)
        self.program['u_resolution'].write(glm.vec2(self.app.WIN_SIZE))
        self.depth_texture = self.app.mesh.texture.textures['depth_texture']
        self.program['shadowMap'] = 1
        self.depth_texture.use(location=1)
        self.shadow_vao = self.app.mesh.vao.vaos['shadow_' + self.vao_name]
        self.shadow_program = self.shadow_vao.program
        self.shadow_program['m_proj'].write(self.camera.m_proj)
        self.shadow_program['m_view_light'].write(self.app.light.m_view_light)
        self.shadow_program['m_model'].write(self.m_model)
        self.texture = self.app.mesh.texture.textures[self.tex_id]
        self.program['u_texture_0'] = 0
        self.texture.use(location=0)
        self.program['m_proj'].write(self.camera.m_proj)
        self.program['m_view'].write(self.camera.m_view)
        self.program['m_model'].write(self.m_model)
        self.program['light.position'].write(self.app.light.position)
        self.program['light.Ia'].write(self.app.light.Ia)
        self.program['light.Id'].write(self.app.light.Id)
        self.program['light.Is'].write(self.app.light.Is)


class Cube(ExtendedBaseModel):
    def __init__(self, app, vao_name='cube', tex_id=0, pos=(0, 0, 0), rot=(0, 0, 0), scale=(1, 1, 1)):
        super().__init__(app, vao_name, tex_id, pos, rot, scale)
class MovingCube(Cube):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
    def update(self):
        self.m_model = self.get_model_matrix()
        super().update()
class OBJWavefront(ExtendedBaseModel):
    def __init__(self, app, vao_name='objw', tex_id='objw',
                 pos=(0, 0, 0), rot=(0, 0, 0), scale=(1, 1, 1)):
        super().__init__(app, vao_name, tex_id, pos, rot, scale)
class SkyBox(BaseModel):
    def __init__(self, app, vao_name='skybox', tex_id='skybox',
                 pos=(0, 0, 0), rot=(0, 0, 0), scale=(1, 1, 1)):
        super().__init__(app, vao_name, tex_id, pos, rot, scale)
        self.on_init()
    def update(self):
        self.program['m_view'].write(glm.mat4(glm.mat3(self.camera.m_view)))
    def on_init(self):
        # texture
        self.texture = self.app.mesh.texture.textures[self.tex_id]
        self.program['u_texture_skybox'] = 0
        self.texture.use(location=0)
        # mvp
        self.program['m_proj'].write(self.camera.m_proj)
        self.program['m_view'].write(glm.mat4(glm.mat3(self.camera.m_view)))
class AdvancedSkyBox(BaseModel):
    def __init__(self, app, vao_name='advanced_skybox', tex_id='skybox',
                 pos=(0, 0, 0), rot=(0, 0, 0), scale=(1, 1, 1)):
        super().__init__(app, vao_name, tex_id, pos, rot, scale)
        self.on_init()
    def update(self):
        m_view = glm.mat4(glm.mat3(self.camera.m_view))
        self.program['m_invProjView'].write(glm.inverse(self.camera.m_proj * m_view))
    def on_init(self):
        self.texture = self.app.mesh.texture.textures[self.tex_id]
        self.program['u_texture_skybox'] = 0
        self.texture.use(location=0)



















