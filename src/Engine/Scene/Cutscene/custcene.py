import glm
from collections import deque

class Cutscene:
    def __init__(self, camera, speed_multiplier=1.0):
        """
        Initialize the Cutscene with a camera and an optional speed multiplier.
        The speed multiplier modifies the overall speed of the cutscene transitions.
        """
        self.camera = camera
        self.keyframes = deque()
        self.current_keyframe = None
        self.time = 0
        self.is_playing = False
        self.ease_func = self.ease_in_out_cubic
        self.speed_multiplier = speed_multiplier
    
    def add_keyframe(self, position, target, duration):
        """
        Add a keyframe to the cutscene.
        A keyframe defines a camera position, where it should look, and how long the transition lasts.
        """
        keyframe = {
            'position': glm.vec3(position),
            'target': glm.vec3(target),
            'duration': duration,
        }
        self.keyframes.append(keyframe)
    
    def play(self):
        """
        Start the cutscene by setting the first keyframe.
        """
        if self.keyframes:
            self.is_playing = True
            self.time = 0
            self.current_keyframe = self.keyframes.popleft()
    
    def stop(self):
        self.is_playing = False
        self.current_keyframe = None
        self.time = 0
    
    def update(self, dt):
        if not self.is_playing or not self.current_keyframe:
            return
        self.time += dt * self.speed_multiplier
        if self.keyframes:
            next_keyframe = self.keyframes[0]
        else:
            self.stop()
            return
        t = min(self.time / self.current_keyframe['duration'], 1.0)
        t = self.ease_func(t)
        self.camera.position = glm.mix(self.current_keyframe['position'], next_keyframe['position'], t)
        target = glm.mix(self.current_keyframe['target'], next_keyframe['target'], t)
        self.camera.look_at(target)
        if self.time >= self.current_keyframe['duration']:
            self.current_keyframe = self.keyframes.popleft()
            self.time = 0
    
    @staticmethod
    def ease_in_out_cubic(t):
        if t < 0.5:
            return 4 * t * t * t
        else:
            return 1 - pow(-2 * t + 2, 3) / 2

    def set_speed(self, speed_multiplier):
        self.speed_multiplier = speed_multiplier
