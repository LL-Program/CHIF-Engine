import numpy as np
from CHIFEngineMain.Utilities import *
from .RenderOptions import RenderOption
from .Renderer import Renderer
class ImageRenderData:
    def __init__(self):
        self.image_texture = None
        self.position = Float3(0.0, 0.0, 0.0)
        self.size = Float3(100.0, 100.0, 1.0)
        self.render_count = 0

    def set_image(self, texture, position, size):
        self.image_texture = texture
        self.position = position
        self.size = size
        self.render_count = 1