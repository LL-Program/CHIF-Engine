import moderngl as mgl
import pygame as pg
import sys
import numpy as np
from model import *
from camera import Camera
from light import Light
from mesh import Mesh
from scene import Scene
from scene_renderer import SceneRenderer
from Utils.audiomanager import AudioManager
from UI.UIManager import UIManager
from Logic.black_hole import BlackHole
from data_collector import FPSLogger

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
        self.black_hole = BlackHole(self)
        # 2D text rendering setup
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
        # Render text to a Pygame surface
        text_surface = self.font.render(text, True, (255, 255, 255))  # White text
        text_data = pg.image.tostring(text_surface, "RGBA", True)
        width, height = text_surface.get_size()

        # Create or update ModernGL texture
        if self.text_texture is None or self.text_texture.size != (width, height):
            if self.text_texture is not None:
                self.text_texture.release()  # Release the old texture if it exists
            self.text_texture = self.ctx.texture((width, height), 4, text_data)
        else:
            self.text_texture.write(text_data)  # Update the existing texture

        self.text_texture.use(location=0)

        # Update the quad vertices based on text size and position
        vertices = np.array([
            x, y, 0.0, 1.0,  # Bottom-left
            x + width, y, 1.0, 1.0,  # Bottom-right
            x + width, y + height, 1.0, 0.0,  # Top-right
            x, y + height, 0.0, 0.0   # Top-left
        ], dtype='f4')
        self.quad_vbo.write(vertices)

        # Set the screen size uniform
        self.prog['screen_size'].value = self.WIN_SIZE

        # Render the quad with the text texture
        self.quad_vao.render(mgl.TRIANGLES)

    def render(self):
        # Clear the screen
        self.ctx.clear(color=(0.08, 0.16, 0.18))

        # Render the scene
        self.scene_renderer.render()

        # Render FPS text if below 60
        if self.clock.get_fps() < 60:
            self.render_text(f"FPS: {self.clock.get_fps():.2f}", 10, 10)

        # Log FPS data
        self.Fps_Logger.log_fps(self.clock.get_fps())
        #self.black_hole.render()

        # Flip the display buffer
        pg.display.flip()



    def get_time(self):
        self.time = pg.time.get_ticks() * 0.0001

    def update_window_size(self):
        if self.WIN_SIZE != pg.display.get_surface().get_size():
            pg.display.set_mode(self.WIN_SIZE, flags=pg.OPENGL | pg.DOUBLEBUF)
            print(f"Window resized to: {self.WIN_SIZE}")

    def run(self):
        while True:
            self.get_time()
            self.check_events()
            self.camera.update()
            self.render()
            print(f"FPS: {self.clock.get_fps()} PHYSICS = {self.physics}")
            self.delta_time = self.clock.tick()
    
if __name__ == '__main__':
    app = CHIFEngine()
    app.run()
