import moderngl as mgl
import pygame as pg
import sys
from Models.model import *
from Actor.camera import Camera
from Physics.light import Light
from Models.mesh import Mesh
from Scene.scene import Scene
from Scene.scene_renderer import SceneRenderer
from Utils.audiomanager import AudioManager
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
        self.collect_data = True
        """1-0 Object INIT"""
        self.light = Light()
        self.light2 = Light((0,0,0),(1,2,2))
        self.camera = Camera(self)
        self.mesh = Mesh(self)
        self.scene = Scene(self)
        self.AudioManager = AudioManager(self)
        self.scene_renderer = SceneRenderer(self)
        self.SpexConnector = SpexConnector(self)
        self.ObjectManager = ObjectManager(self)
        """1-1 Cutscene INIT"""
        self.Cutscene = Cutscene(self.camera)
    def check_events(self):
        for event in pg.event.get():
            if event.type == pg.QUIT or (event.type == pg.KEYDOWN and event.key == pg.K_ESCAPE):
                self.mesh.destroy()
                self.scene_renderer.destroy()
                pg.quit()
                sys.exit()
    def render(self):
        self.ctx.clear(color=(0.08, 0.16, 0.18))
        self.scene_renderer.render()
        pg.display.flip()
    def get_time(self):
        self.time = pg.time.get_ticks() * 0.0001
    def update_window_size(self):
        if self.WIN_SIZE != pg.display.get_surface().get_size():
            pg.display.set_mode(self.WIN_SIZE, flags=pg.OPENGL | pg.DOUBLEBUF)
    def run(self):
        self.Cutscene.play()
        while True:
            self.get_time()
            self.check_events()
            self.camera.update()
            self.render()
            self.delta_time = self.clock.tick(1000)
            self.Cutscene.update(self.delta_time)
if __name__ == '__main__':
    app = CHIFEngine()
    app.run()