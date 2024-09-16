import glm
import pygame as pg

class Camera:
    def __init__(self, app, position=(0, 0, 4), yaw=-90, pitch=0):
        self.app = app
        self.aspect_ratio = app.WIN_SIZE[0] / app.WIN_SIZE[1]
        self.position = glm.vec3(position)
        self.up = glm.vec3(0, 1, 0)
        self.right = glm.vec3(1, 0, 0)
        self.forward = glm.vec3(0, 0, -1)
        self.yaw = yaw
        self.pitch = pitch
        self.fov = 50
        self.near = 0.1
        self.far = 1000
        self.speed = 0.005
        self.sensitivity = 0.04
        self.m_view = self.get_view_matrix()
        self.m_proj = self.get_projection_matrix()
        self.collision_on = False
        # Camera's AABB size (can be adjusted based on camera's scale in the world)
        self.bounding_box_size = glm.vec3(0.5, 1.5, 0.5)
    def get_view_matrix(self):
        return glm.lookAt(self.position, self.position + self.forward, self.up)
    def get_projection_matrix(self):
        return glm.perspective(glm.radians(self.fov), self.aspect_ratio, self.near, self.far)
    def rotate(self):
        rel_x, rel_y = pg.mouse.get_rel()
        self.yaw += rel_x * self.sensitivity
        self.pitch -= rel_y * self.sensitivity
        self.pitch = max(-89, min(89, self.pitch))
    def update_camera_vectors(self):
        yaw, pitch = glm.radians(self.yaw), glm.radians(self.pitch)
        self.forward.x = glm.cos(yaw) * glm.cos(pitch)
        self.forward.y = glm.sin(pitch)
        self.forward.z = glm.sin(yaw) * glm.cos(pitch)
        self.forward = glm.normalize(self.forward)
        self.right = glm.normalize(glm.cross(self.forward, glm.vec3(0, 1, 0)))
        self.up = glm.normalize(glm.cross(self.right, self.forward))
    def get_aabb(self):
        """Return the Axis-Aligned Bounding Box (AABB) for the camera."""
        half_size = self.bounding_box_size * 0.5
        min_bounds = self.position - half_size
        max_bounds = self.position + half_size
        return min_bounds, max_bounds
    def move(self):
        velocity = self.speed * self.app.delta_time
        keys = pg.key.get_pressed()
        # Save the original position before moving
        original_position = glm.vec3(self.position)
        # Move the camera based on key input
        if keys[pg.K_w]:
            self.position += self.forward * velocity
        if keys[pg.K_s]:
            self.position -= self.forward * velocity
        if keys[pg.K_a]:
            self.position -= self.right * velocity
        if keys[pg.K_d]:
            self.position += self.right * velocity
        if keys[pg.K_q]:
            self.position += self.up * velocity
        if keys[pg.K_e]:
            self.position -= self.up * velocity
        # Check for collisions after moving
        if self.app.scene:
            if self.collision_on:
                if self.check_collisions():
                    self.position = original_position  # Revert to original position if collision occurs
    def check_collisions(self):
        """Check for collisions between the camera and objects in the scene."""
        camera_min, camera_max = self.get_aabb()
        for obj in self.app.scene.objects:
            obj_min, obj_max = obj.get_aabb()
            if self.check_aabb_collision(camera_min, camera_max, obj_min, obj_max):
                return True
        return False
    def check_aabb_collision(self, a_min, a_max, b_min, b_max):
        """Check if two AABBs collide."""
        return (a_min.x <= b_max.x and a_max.x >= b_min.x and
                a_min.y <= b_max.y and a_max.y >= b_min.y and
                a_min.z <= b_max.z and a_max.z >= b_min.z)
    def update(self):
        self.move()
        self.rotate()
        self.update_camera_vectors()
        self.m_view = self.get_view_matrix()


















