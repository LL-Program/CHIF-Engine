from model import *
from Utils.objectmanager import ObjectManager
import Logic.physics as physics

class Scene:
    def __init__(self, app):
        self.app = app
        self.objects = []
        self.load()
        self.skybox = AdvancedSkyBox(app)
        self.ObjectManager = ObjectManager(app, self)
    def add_object(self, obj):
        self.objects.append(obj)
    def load(self):
        app = self.app
        add = self.add_object
        add(OBJWavefront(app, scale=(1, 1, 1), pos=(0, -1, -10)))
    def update(self):
        for obj in self.objects:
            obj.update_physics(self.app.delta_time)
        physics.check_collisions(self.objects, self.app.camera)
