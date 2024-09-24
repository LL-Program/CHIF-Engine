import moderngl as mgl
import pygame as pg
import sys
import numpy as np
from Models.model import *
from Actor.camera import Camera
from Physics.light import Light
from Models.mesh import Mesh
from Scene.scene import Scene
from Scene.scene_renderer import SceneRenderer
from Utils.audiomanager import AudioManager
from UI.UIManager import UIManager
from Telemetry.data_collector import FPSLogger
from Spex.CHIFConnector import SpexConnector
from Scene.Cutscene.custcene import Cutscene
from Models.object_manager import ObjectManager
class CHIFEngine:
    def __init__(self, win_size=(1920, 1080)):
        pg.init()
        self.WIN_SIZE = win_size
        pg.display.gl_set_attribute(pg.GL_CONTEXT_MAJOR_VERSION, 3)
        pg.display.gl_set_attribute(pg.GL_CONTEXT_MINOR_VERSION, 3)
        pg.display.gl_set_attribute(pg.GL_CONTEXT_PROFILE_MASK, pg.GL_CONTEXT_PROFILE_CORE)
        pg.display.set_mode(self.WIN_SIZE, flags=pg.OPENGL | pg.DOUBLEBUF)
        pg.event.set_grab(True)
        pg.mouse.set_visible(False)
        pg.display.set_caption("CHIF Engine")
        self.ctx = mgl.create_context()
        self.ctx.enable(flags=mgl.DEPTH_TEST | mgl.CULL_FACE)
        self.ctx.enable(mgl.BLEND)
        self.ctx.blend_func = mgl.SRC_ALPHA, mgl.ONE_MINUS_SRC_ALPHA
        self.clock = pg.time.Clock()
        self.time = 0 
        self.delta_time = 0
        self.physics = False
        #data collection
        self.collect_data = True
        if self.collect_data:
            self.Fps_Logger = FPSLogger()
        # Objects initialization
        self.light = Light()
        self.camera = Camera(self)
        self.mesh = Mesh(self)
        self.scene = Scene(self)
        self.AudioManager = AudioManager(self)
        self.scene_renderer = SceneRenderer(self)
        self.UIManager = UIManager(self)
        self.SpexConnector = SpexConnector(self)
        self.ObjectManager = ObjectManager(self)
        #Editor
        #Cutscene
        self.Cutscene = Cutscene(self.camera)
        self.Cutscene.set_speed(0.0005)
        #self.camera.focus_on_point((0, -1, -100), smooth=False, speed=0.1)
        #self.Cutscene.add_keyframe(position=(0, 5, 10), target=(0, 0, 0), duration=3)
        # self.Cutscene.add_keyframe(position=(5, 5, 10), target=(0, 0, 0), duration=3)
        #self.AudioManager.load_music("Ressources/Music/TheEnd.mp3")
        #self.AudioManager.play_music(-1)
        self.font = pg.font.Font(None, 36)
        self.prog = self.ctx.program(
            vertex_shader="""
            #version 330
            in vec2 in_pos;
            in vec2 in_tex;
            out vec2 v_tex;
            uniform vec2 screen_size;
            void main() {
                v_tex = in_tex;
                vec2 pos = in_pos / screen_size * 2.0 - 1.0;
                gl_Position = vec4(pos.x, -pos.y, 0.0, 1.0);
            }
            """,
            fragment_shader="""
            #version 330
            in vec2 v_tex;
            out vec4 fragColor;
            uniform sampler2D text_texture;
            void main() {
                fragColor = texture(text_texture, v_tex);
            }
            """,
        )
        self.quad_vbo = self.ctx.buffer(
            np.array([
                0.0, 0.0, 0.0, 1.0, 
                1.0, 0.0, 1.0, 1.0,
                1.0, 1.0, 1.0, 0.0,
                0.0, 1.0, 0.0, 0.0
            ], dtype='f4')
        )
        self.quad_ibo = self.ctx.buffer(
            np.array([0, 1, 2, 2, 3, 0], dtype='i4')
        )
        self.quad_vao = self.ctx.vertex_array(
            self.prog,
            [(self.quad_vbo, '2f 2f', 'in_pos', 'in_tex')],
            self.quad_ibo
        )
        self.text_texture = None

    def check_events(self):
        for event in pg.event.get():
            if event.type == pg.QUIT or (event.type == pg.KEYDOWN and event.key == pg.K_ESCAPE):
                self.mesh.destroy()
                self.scene_renderer.destroy()
                pg.quit()
                sys.exit()
            if event.type == pg.KEYDOWN and event.key == pg.K_LSHIFT:
                self.UIManager.openGraphicsSettings()

    def render_text(self, text, x, y):
        """Renders text using Pygame and ModernGL"""
        text_surface = self.font.render(text, True, (255, 255, 255))
        text_data = pg.image.tostring(text_surface, "RGBA", True)
        width, height = text_surface.get_size()

        if self.text_texture is None or self.text_texture.size != (width, height):
            if self.text_texture is not None:
                self.text_texture.release()
            self.text_texture = self.ctx.texture((width, height), 4, text_data)
        else:
            self.text_texture.write(text_data)

        self.text_texture.use(location=0)
        vertices = np.array([
            x, y, 0.0, 1.0,
            x + width, y, 1.0, 1.0,
            x + width, y + height, 1.0, 0.0,
            x, y + height, 0.0, 0.0
        ], dtype='f4')
        self.quad_vbo.write(vertices)
        self.prog['screen_size'].value = self.WIN_SIZE
        self.quad_vao.render(mgl.TRIANGLES)

    def render(self):
        self.ctx.clear(color=(0.08, 0.16, 0.18))
        self.scene_renderer.render()
        # Render FPS text if below 60
        if self.clock.get_fps() < 60:
            self.render_text(f"FPS: {self.clock.get_fps():.2f}", 10, 10)
        # Log FPS data
        self.Fps_Logger.log_fps(self.clock.get_fps())
        #self.black_hole.render()
        pg.display.flip()

    def get_time(self):
        self.time = pg.time.get_ticks() * 0.0001

    def update_window_size(self):
        if self.WIN_SIZE != pg.display.get_surface().get_size():
            pg.display.set_mode(self.WIN_SIZE, flags=pg.OPENGL | pg.DOUBLEBUF)
            print(f"Window resized to: {self.WIN_SIZE}")

    def run(self):
        self.Cutscene.play()
        while True:
            self.get_time()
            self.check_events()
            self.camera.update()
            self.render()
            #print(f"FPS: {self.clock.get_fps()} PHYSICS = {self.physics}")
            self.delta_time = self.clock.tick(1000)
            self.Cutscene.update(self.delta_time)
    
if __name__ == '__main__':
    app = CHIFEngine()
    app.run()