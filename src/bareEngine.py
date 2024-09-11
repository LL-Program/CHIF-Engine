import customtkinter as ct
import pygame as pg
import moderngl as mgl
import numpy as np
import sys
from model import *
from camera import Camera
from light import Light
from mesh import Mesh
from scene import Scene
from scene_renderer import SceneRenderer
from audiomanager import AudioManager
from UI.UIManager import UIManager

class PygameFrame(ct.CTkFrame):
    def __init__(self, master, engine, **kwargs):
        super().__init__(master, **kwargs)
        self.engine = engine
        self.canvas = ct.CTkCanvas(self, bg='black')
        self.canvas.pack(fill='both', expand=True)
        self.canvas.bind("<Configure>", self.on_resize)
        self.update_pygame()

    def on_resize(self, event):
        # Resize the Pygame window to match the canvas size
        self.engine.update_window_size(self.canvas.winfo_width(), self.canvas.winfo_height())
        self.update_pygame()

    def update_pygame(self):
        self.engine.run()

class CHIFEngine:
    def __init__(self, win_size=(1600, 900)):
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
        self.clock = pg.time.Clock()
        self.time = 0
        self.delta_time = 0
        self.physics = False
        
        # Initialize game objects
        self.light = Light()
        self.camera = Camera(self)
        self.mesh = Mesh(self)
        self.scene = Scene(self)
        self.AudioManager = AudioManager(self)
        self.scene_renderer = SceneRenderer(self)
        self.UIManager = UIManager(self)
        self.font = pg.font.Font(None, 36)

        # Prepare ModernGL program for rendering 2D textures (text)
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
            """
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

    def check_events(self):
        for event in pg.event.get():
            if event.type == pg.QUIT or (event.type == pg.KEYDOWN and event.key == pg.K_ESCAPE):
                pg.quit()
                sys.exit()
            if event.type == pg.KEYDOWN and event.key == pg.K_LSHIFT:
                self.UIManager.openGraphicsSettings()

    def render_text(self, text, x, y):
        text_surface = self.font.render(text, True, (255, 255, 255))
        text_data = pg.image.tostring(text_surface, "RGBA", True)
        width, height = text_surface.get_size()

        texture = self.ctx.texture((width, height), 4, text_data)
        texture.use(location=0)

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
        pg.display.flip()

    def get_time(self):
        self.time = pg.time.get_ticks() * 0.001

    def update_window_size(self, width, height):
        self.WIN_SIZE = (width, height)
        pg.display.set_mode(self.WIN_SIZE, flags=pg.OPENGL | pg.DOUBLEBUF)
        print(f"Window resized to: {self.WIN_SIZE}")

    def run(self):
        while True:
            self.get_time()
            self.check_events()
            self.camera.update()
            self.render()
            self.delta_time = self.clock.tick(60)

class MainApp(ct.CTk):
    def __init__(self):
        super().__init__()
        self.title("CHIF Engine with CustomTkinter")
        self.geometry("1600x900")

        # Initialize the engine
        self.engine = CHIFEngine()

        # Create a frame for the Pygame engine
        self.pygame_frame = PygameFrame(master=self, engine=self.engine)
        self.pygame_frame.pack(fill='both', expand=True)

        # Add additional customtkinter widgets
        self.label = ct.CTkLabel(master=self, text="This is a CustomTkinter Label")
        self.label.pack(pady=20)

if __name__ == "__main__":
    app = MainApp()
    app.mainloop()
