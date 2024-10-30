from enum import Enum

from CHIFEngineMain.Common import logger
from CHIFEngineMain.Utilities import *
from CHIFEngineMain.Common.Constants import DEBUG_MODE

class BlendMode(Enum):
    BLEND = 0
    ADDITIVE = 1
    MULTIPLY = 2
    SUBTRACT = 3


class RenderOption:
    RENDER_LIGHT_PROBE = False
    RENDER_ONLY_ATMOSPHERE = False
    RENDER_FONT = True
    RENDER_STATIC_ACTOR = True
    RENDER_SKELETON_ACTOR = True
    RENDER_ATMOSPHERE = True
    RENDER_OCEAN = True
    RENDER_EFFECT = True
    RENDER_COLLISION = True
    RENDER_OBJECT_ID = True
    RENDER_IMAGE = True
    #DEBUGMODE
    if DEBUG_MODE:
        RENDER_GIZMO = True
        RENDER_DEBUG_LINE = True
    else:
        RENDER_GIZMO = False
        RENDER_DEBUG_LINE = False


class RenderingType(AutoEnum):
    DEFERRED_RENDERING = ()
    FORWARD_RENDERING = ()
    LIGHT_PRE_PASS = ()
    COUNT = ()


class RenderGroup(AutoEnum):
    STATIC_ACTOR = ()
    SKELETON_ACTOR = ()
    COUNT = ()


class RenderMode(AutoEnum):
    GBUFFER = ()
    FORWARD_SHADING = ()
    SHADOW = ()
    OBJECT_ID = ()
    SELECTED_OBJECT = ()
    GIZMO = ()
    COUNT = ()


class RenderOptionManager(Singleton):
    def __init__(self):
        logger.info("Create " + GetClassName(self))
        self.rendering_type = RenderingType.DEFERRED_RENDERING

        self.core_manager = None

    def initialize(self, core_manager):
        self.core_manager = core_manager

    def set_rendering_type(self, rendering_type):
        self.rendering_type = RenderingType.convert_index_to_enum(rendering_type)
