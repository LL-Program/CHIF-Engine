import glm
import numpy as np
from Input.gizmo import *
import pygame as pg

class Manipulator:
    def __init__(self, app):
        self.app = app
        self.gizmo = Gizmo(app)
        self.selected_object = None
        self.selected_axis = None
        self.dragging = False
        self.drag_start_position = None
        self.initial_object_position = None

    def update(self):
        if self.app.input.mouse_button_down:
            axis = self.check_axis_selection()
            if axis:
                self.selected_axis = axis
                self.start_drag()

        if self.dragging:
            delta = self.calculate_drag_delta()
            if self.selected_object:
                self.drag_object(delta)

        if self.app.input.mouse_button_up:
            self.stop_drag()

    def check_axis_selection(self):
        ray_origin, ray_direction = self.get_ray_from_mouse()
        if self.check_axis_intersection(self.get_axis_bounds('x'), ray_origin, ray_direction):
            return 'x'
        elif self.check_axis_intersection(self.get_axis_bounds('y'), ray_origin, ray_direction):
            return 'y'
        elif self.check_axis_intersection(self.get_axis_bounds('z'), ray_origin, ray_direction):
            return 'z'
        
        return None

    def start_drag(self):
        self.dragging = True
        self.drag_start_position = self.get_mouse_position_in_world()
        self.initial_object_position = self.selected_object.pos.copy()

    def drag_object(self, delta):
        if self.selected_axis == 'x':
            self.selected_object.pos.x = self.initial_object_position.x + delta.x
        elif self.selected_axis == 'y':
            self.selected_object.pos.y = self.initial_object_position.y + delta.y
        elif self.selected_axis == 'z':
            self.selected_object.pos.z = self.initial_object_position.z + delta.z

    def stop_drag(self):
        self.dragging = False
        self.selected_axis = None

    def render(self):
        if self.selected_object:
            self.gizmo.render(glm.translate(glm.mat4(), self.selected_object.pos))

    def calculate_drag_delta(self):
        current_position = self.get_mouse_position_in_world()
        delta = current_position - self.drag_start_position
        return delta

    def get_axis_bounds(self, axis):
        size = 1.0
        if axis == 'x':
            return np.array([[-size, 0, 0], [size, 0, 0]])
        elif axis == 'y':
            return np.array([[0, -size, 0], [0, size, 0]])
        elif axis == 'z':
            return np.array([[0, 0, -size], [0, 0, size]])
        return None

    def check_axis_intersection(self, axis_bounds, ray_origin, ray_direction):
        return self.line_intersects_ray(axis_bounds[0], axis_bounds[1], ray_origin, ray_direction)

    def line_intersects_ray(self, line_start, line_end, ray_origin, ray_direction):
        line_start = np.array(line_start, dtype=np.float64)
        line_end = np.array(line_end, dtype=np.float64)
        ray_origin = np.array(ray_origin, dtype=np.float64)
        ray_direction = np.array(ray_direction, dtype=np.float64)

        line_direction = line_end - line_start
        line_length = np.linalg.norm(line_direction)

        if line_length == 0:
            return False

        line_direction /= line_length
        return True

    def get_ray_from_mouse(self):
        mouse_x, mouse_y = self.app.input_manager.get_mouse_position()
        width, height = self.app.WIN_SIZE

        x = (2.0 * mouse_x) / width - 1.0
        y = 1.0 - (2.0 * mouse_y) / height
        ray_nds = np.array([x, y, -1.0])
        ray_clip = np.array([ray_nds[0], ray_nds[1], -1.0, 1.0])
        ray_eye = glm.inverse(self.app.camera.get_projection_matrix()) * ray_clip
        ray_eye = np.array([ray_eye[0], ray_eye[1], -1.0])
        ray_world = glm.normalize(ray_eye)

        return self.app.camera.position, ray_world


    def get_mouse_position_in_world(self):
        ray_origin, ray_direction = self.get_ray_from_mouse()
        distance = 5.0
        return ray_origin + ray_direction * distance