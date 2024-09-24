from Models.model import *
import Logic.physics as physics

class Scene:
    def __init__(self, app):
        self.app = app
        self.objects = []
        self.load()
        self.skybox = AdvancedSkyBox(app)
    def add_object(self, obj):
        self.objects.append(obj)
    def load(self):
        app = self.app
        add = self.add_object
    def update(self):
        for obj in self.objects:
            obj.update_physics(self.app.delta_time)
        physics.check_collisions(self.objects, self.app.camera)
