from model import *
from objectmanager import ObjectManager
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
        n, s = 20, 2
        #add(OBJWavefront(app, scale=(1, 1, 1), pos=(0, -1, -10)))
        #add(OBJWavefront(app, scale=(1, 1, 1), pos=(0, -1, -1)))
        self.movingcube = MovingCube(app,pos=(0,12,0),scale=(1,1,1))
        add(self.movingcube)

    def update(self):
        self.movingcube.update()
        for obj in self.objects:
            obj.update_physics(self.app.delta_time)  # Update physics
        physics.check_collisions(self.objects)
