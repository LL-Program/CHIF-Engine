from model import *
from objectmanager import ObjectManager

class Scene:
    def __init__(self, app):
        self.app = app
        self.objects = []
        self.load()
        self.skybox = AdvancedSkyBox(app)
        self.ObjectManager = ObjectManager(app,self)
    def add_object(self, obj):
        self.objects.append(obj)
    def load(self):
        app = self.app
        add = self.add_object
        n, s = 20, 2
        add(OBJWavefront(app, pos=(0, -1, -10)))
    def update(self):
        pass
