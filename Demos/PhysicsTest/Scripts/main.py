import os
import time
import math

import numpy as np
import pybullet
import pybullet_data

from CHIFEngineMain.Common import logger
from CHIFEngineMain.Utilities import Singleton, Float3, Float4
from CHIFEngineMain.src.GameBackend import Keyboard


def change(x):
    return [-x[0], x[2], x[1]]


class ScriptManager(Singleton):
    def __init__(self):
        logger.info("ScriptManager::__init__")
        self.core_manager = None
        self.renderer = None
        self.debug_line_manager = None
        self.game_backend = None
        self.resource_manager = None
        self.scene_manager = None
        self.viewport_manager = None
        self.main_viewport = None
        self.actor_plane = None
        self.actor_spheres = []
        self.actor_suzans = []
        self.physics_client = None
        self.physics_plane = None
        self.physics_suzans = []
        self.physics_spheres = []

    def initialize(self, core_manager):
        logger.info("ScriptManager::initialize")

        self.core_manager = core_manager
        self.renderer = core_manager.renderer
        self.debug_line_manager = core_manager.debug_line_manager
        self.game_backend = core_manager.game_backend
        self.resource_manager = core_manager.resource_manager
        self.scene_manager = core_manager.scene_manager
        self.viewport_manager = core_manager.viewport_manager
        self.main_viewport = core_manager.viewport_manager.main_viewport

        self.resource_manager.open_scene('physics_scene')

        camera_transform = self.scene_manager.main_camera.transform
        camera_transform.set_pos([12.5, 12.5, 13.8])
        camera_transform.set_pitch(5.62)
        camera_transform.set_yaw(0.67)

        model_plane = self.resource_manager.get_model("Cube")
        model_sphere = self.resource_manager.get_model("sphere")
        model_suzan = self.resource_manager.get_model("suzan")
        mesh_count = 10

        self.actor_plane = self.scene_manager.add_object(model=model_plane, pos=[10.0, 0.0, 0.0], scale=[15.0, 0.01, 15.0])

        def get_random_pos():
            pos = np.random.rand(3)
            pos[0] = pos[0] * 2.0 - 1.0
            pos[1] += np.random.rand() * 6.0
            pos[2] = pos[2] * 2.0 - 1.0
            return pos * 2.0

        for i in range(mesh_count):
            actor_sphere = self.scene_manager.add_object(model=model_sphere, pos=get_random_pos(), scale=Float3(0.5, 0.5, 0.5))
            actor_suzan = self.scene_manager.add_object(model=model_suzan, pos=get_random_pos(), scale=Float3(0.5, 0.5, 0.5))
            self.actor_spheres.append(actor_sphere)
            self.actor_suzans.append(actor_suzan)

        self.initialize_physics()

    def initialize_physics(self):
        self.physics_client = pybullet.connect(pybullet.DIRECT)
        pybullet.setAdditionalSearchPath(pybullet_data.getDataPath())
        pybullet.setAdditionalSearchPath(self.resource_manager.project_path)
        pybullet.setGravity(-10, 0, -98)

        self.physics_plane = pybullet.loadURDF(
            "Externals/Physics/plane.urdf",
            change(self.actor_plane.transform.get_pos())
        )

        for actor_sphere in self.actor_spheres:
            physics_sphere = pybullet.loadURDF(
                "Externals/Physics/sphere.urdf",
                change(actor_sphere.transform.get_pos())
            )
            self.physics_spheres.append(physics_sphere)

        for actor_suzan in self.actor_suzans:
            physics_suzan = pybullet.loadURDF(
                "Externals/Physics/suzan.urdf",
                change(actor_suzan.transform.get_pos())
            )
            self.physics_suzans.append(physics_suzan)

    def exit(self):
        logger.info("ScriptManager::exit")

        pybullet.disconnect()

        for actor_sphere in self.actor_spheres:
            self.scene_manager.delete_object(actor_sphere.name)
        self.actor_spheres = []

        for actor_suzan in self.actor_suzans:
            self.scene_manager.delete_object(actor_suzan.name)
        self.actor_suzans = []

        if self.actor_plane is not None:
            self.scene_manager.delete_object(self.actor_plane.name)
            self.actor_plane = None

    def update(self, delta):
        pybullet.stepSimulation()

        self.update_camera(delta)

        for i, actor_suzan in enumerate(self.actor_suzans):
            position, rotation = pybullet.getBasePositionAndOrientation(self.physics_suzans[i])
            actor_suzan.transform.set_pos(change(position))
            actor_suzan.transform.set_rotation(change(pybullet.getEulerFromQuaternion(rotation)))

        for i, actor_sphere in enumerate(self.actor_spheres):
            position, rotation = pybullet.getBasePositionAndOrientation(self.physics_spheres[i])
            actor_sphere.transform.set_pos(change(position))
            actor_sphere.transform.set_rotation(change(pybullet.getEulerFromQuaternion(rotation)))

        self.debug_line_manager.draw_debug_line_3d(Float3(0.0, 0.0, 0.0), Float3(3.0, 0.0, 0.0), Float4(1.0, 0.0, 0.0, 1.0), width=3.0)
        self.debug_line_manager.draw_debug_line_3d(Float3(0.0, 0.0, 0.0), Float3(0.0, 3.0, 0.0), Float4(0.0, 1.0, 0.0, 1.0), width=3.0)
        self.debug_line_manager.draw_debug_line_3d(Float3(0.0, 0.0, 0.0), Float3(0.0, 0.0, 3.0), Float4(0.0, 0.0, 1.0, 1.0), width=3.0)

    def update_camera(self, delta):
        keydown = self.game_backend.get_keyboard_pressed()
        mouse_delta = self.game_backend.mouse_delta
        btn_left, btn_middle, btn_right = self.game_backend.get_mouse_pressed()

        # get camera
        camera = self.scene_manager.main_camera
        camera_transform = camera.transform
        move_speed = camera.move_speed * delta
        pan_speed = camera.pan_speed * delta

        if keydown[Keyboard.LSHIFT]:
            move_speed *= 4.0
            pan_speed *= 4.0

        # camera move pan
        if btn_left and btn_right or btn_middle:
            camera_transform.move_left(-mouse_delta[0] * pan_speed)
            camera_transform.move_up(-mouse_delta[1] * pan_speed)

        # camera rotation
        elif btn_left or btn_right:
            camera_transform.rotation_pitch(mouse_delta[1] * camera.rotation_speed)
            camera_transform.rotation_yaw(-mouse_delta[0] * camera.rotation_speed)

        if keydown[Keyboard.Z]:
            camera_transform.rotation_roll(-camera.rotation_speed * delta)
        elif keydown[Keyboard.C]:
            camera_transform.rotation_roll(camera.rotation_speed * delta)

        # move to view direction ( inverse front of camera matrix )
        if keydown[Keyboard.W] or self.game_backend.wheel_up:
            camera_transform.move_front(-move_speed)
        elif keydown[Keyboard.S] or self.game_backend.wheel_down:
            camera_transform.move_front(move_speed)

        # move to side
        if keydown[Keyboard.A]:
            camera_transform.move_left(-move_speed)
        elif keydown[Keyboard.D]:
            camera_transform.move_left(move_speed)

        # move to up
        if keydown[Keyboard.Q]:
            camera_transform.move_up(move_speed)
        elif keydown[Keyboard.E]:
            camera_transform.move_up(-move_speed)

        if keydown[Keyboard.SPACE]:
            camera_transform.reset_transform()
        if keydown[Keyboard.I]:
            self.core_manager.sound_manager.play_music("TheEnd.mp3")

