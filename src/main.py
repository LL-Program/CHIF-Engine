import pygame as pg
import moderngl as mgl
import sys
from model import *
from camera import Camera
from light import Light
from mesh import Mesh
from scene import Scene
from scene_renderer import SceneRenderer
from audiomanager import AudioManager

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
        #Objects -- 8.9.24
        self.light = Light()
        self.camera = Camera(self)
        self.mesh = Mesh(self)
        self.scene = Scene(self)
        self.AudioManager = AudioManager(self)
        self.scene_renderer = SceneRenderer(self)
        #new 2d
        self.font = pg.font.Font(None, 36)  # Default font, size 36

        # Prepare a ModernGL program for rendering 2D textures (text)
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

        # Vertex buffer for rendering the quad (the text)
        self.quad_vbo = self.ctx.buffer(
            np.array([
                # x, y, tex_x, tex_y
                0.0, 0.0, 0.0, 1.0,  # Bottom-left
                1.0, 0.0, 1.0, 1.0,  # Bottom-right
                1.0, 1.0, 1.0, 0.0,  # Top-right
                0.0, 1.0, 0.0, 0.0   # Top-left
            ], dtype='f4')
        )

        # Element buffer to define the quad (2 triangles)
        self.quad_ibo = self.ctx.buffer(
            np.array([0, 1, 2, 2, 3, 0], dtype='i4')
        )

        self.quad_vao = self.ctx.vertex_array(
            self.prog,
            [
                (self.quad_vbo, '2f 2f', 'in_pos', 'in_tex')
            ],
            self.quad_ibo
        )
    def check_events(self):
        for event in pg.event.get():
            if event.type == pg.QUIT or (event.type == pg.KEYDOWN and event.key == pg.K_ESCAPE):
                self.mesh.destroy()
                self.scene_renderer.destroy()
                pg.quit()
                sys.exit()
    def render_text(self, text, x, y):
        """Renders text using Pygame and ModernGL"""
        # Render text to a Pygame surface
        text_surface = self.font.render(text, True, (255, 255, 255))  # White text
        text_data = pg.image.tostring(text_surface, "RGBA", True)
        width, height = text_surface.get_size()

        # Create a ModernGL texture from the Pygame surface
        texture = self.ctx.texture((width, height), 4, text_data)
        texture.use(location=0)

        # Adjust the quad vertices based on text size and position
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
        self.ctx.clear(color=(0.08, 0.16, 0.18))
        self.scene_renderer.render()
        self.render_text(f"FPS: {self.clock.get_fps():.2f}", 10, 10)
        pg.display.flip()

    def get_time(self):
        self.time = pg.time.get_ticks() * 0.001

    def run(self):
        while True:
            self.get_time()
            self.check_events()
            self.camera.update()
            self.render()
            self.delta_time = self.clock.tick(60)


if __name__ == '__main__':
    app = CHIFEngine()
    app.run()






























