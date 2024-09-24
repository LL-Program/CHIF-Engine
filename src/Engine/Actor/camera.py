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
        self.far = 100000
        self.speed = 0.005
        self.sensitivity = 0.04
        self.m_view = self.get_view_matrix()
        self.m_proj = self.get_projection_matrix()
        self.collision_on = False
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
        half_size = self.bounding_box_size * 0.5
        min_bounds = self.position - half_size
        max_bounds = self.position + half_size
        return min_bounds, max_bounds

    def move(self):
        velocity = self.speed * self.app.delta_time
        keys = pg.key.get_pressed()
        original_position = glm.vec3(self.position)

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

        if self.app.scene and self.collision_on and self.check_collisions():
            self.position = original_position

    def check_collisions(self):
        camera_min, camera_max = self.get_aabb()
        for obj in self.app.scene.objects:
            obj_min, obj_max = obj.get_aabb()
            if self.check_aabb_collision(camera_min, camera_max, obj_min, obj_max):
                return True
        return False

    def check_aabb_collision(self, a_min, a_max, b_min, b_max):
        return (a_min.x <= b_max.x and a_max.x >= b_min.x and
                a_min.y <= b_max.y and a_max.y >= b_min.y and
                a_min.z <= b_max.z and a_max.z >= b_min.z)

    def update(self):
        self.move()
        self.rotate()
        self.update_camera_vectors()
        self.m_view = self.get_view_matrix()

    def focus_on_point(self, target_point, smooth=False, speed=0.05):
        """
        Focus the camera on a specific target point. Optionally, smoothly transition to it.

        :param target_point: The point (vec3) to focus on.
        :param smooth: Whether to smoothly transition to the focus.
        :param speed: How fast to transition if smooth is True.
        """
        target_point = glm.vec3(target_point)
        direction_to_target = glm.normalize(target_point - self.position)

        if smooth:
            self.forward = glm.mix(self.forward, direction_to_target, speed)
        else:
            self.forward = direction_to_target

        self.m_view = glm.lookAt(self.position, target_point, self.up)
        self.update_camera_vectors()
    def look_at(self, target):
        """
        Update the camera's forward vector to look at the given target.
        """
        direction = glm.normalize(target - self.position)
        self.forward = direction
        self.update_camera_vectors()
class Player(Camera):
    def __init__(self, app, position=(0, 0, 3), yaw=-90, pitch=0):
        super().__init__(app, position, yaw, pitch)
        self.walk_speed = 0.005
        self.sprint_speed = 0.01
        self.current_speed = self.walk_speed
        self.is_sprinting = False
        self.gravity = 0.00001
        self.jump_force = 0.02
        self.velocity_y = 0
        self.is_jumping = False
        self.is_grounded = True

    def handle_movement_input(self):
        """Handles movement and sprinting input."""
        keys = pg.key.get_pressed()
        if keys[pg.K_LSHIFT]:
            self.is_sprinting = True
        else:
            self.is_sprinting = False
        self.current_speed = self.sprint_speed if self.is_sprinting else self.walk_speed
        velocity = self.current_speed * self.app.delta_time
        forward_flat = glm.normalize(glm.vec3(self.forward.x, 0, self.forward.z))

        if keys[pg.K_w]:
            self.position += forward_flat * velocity
        if keys[pg.K_s]:
            self.position -= forward_flat * velocity
        if keys[pg.K_a]:
            self.position -= self.right * velocity
        if keys[pg.K_d]:
            self.position += self.right * velocity

    def handle_jump_input(self):
        """Handles jump input and applies gravity."""
        keys = pg.key.get_pressed()
        if keys[pg.K_SPACE] and self.is_grounded:
            self.is_jumping = True
            self.velocity_y = self.jump_force
            self.is_grounded = False
        if not self.is_grounded:
            self.velocity_y -= self.gravity * self.app.delta_time
            self.position.y += self.velocity_y * self.app.delta_time
        if self.position.y <= 0:
            self.position.y = 0
            self.is_grounded = True
            self.velocity_y = 0
    def move(self):
        """Override the move method to include additional player-specific input."""
        self.handle_movement_input()
        self.handle_jump_input()

    def update(self):
        """Updates player movement, sprinting, jumping, and camera orientation."""
        self.move()
        self.rotate()
        self.update_camera_vectors()
        self.m_view = self.get_view_matrix()