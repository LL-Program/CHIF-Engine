import pygame as pg

class AudioManager:
    def __init__(self,app):
        # Initialize the mixer module
        pg.mixer.init()
        self.sounds = {}
        self.music_loaded = False

    def load_sound(self, name, filepath):
        """
        Load a sound effect into the manager.
        """
        try:
            sound = pg.mixer.Sound(filepath)
            self.sounds[name] = sound
        except pg.error as e:
            print(f"Error loading sound {filepath}: {e}")

    def play_sound(self, name, loops=0):
        """
        Play a sound effect. Specify how many times to loop (-1 for infinite).
        """
        if name in self.sounds:
            self.sounds[name].play(loops=loops)
        else:
            print(f"Sound '{name}' not found!")

    def stop_sound(self, name):
        """
        Stop a specific sound effect from playing.
        """
        if name in self.sounds:
            self.sounds[name].stop()

    def set_sound_volume(self, name, volume):
        """
        Set the volume of a specific sound effect (0.0 to 1.0).
        """
        if name in self.sounds:
            self.sounds[name].set_volume(volume)

    def load_music(self, filepath):
        """
        Load background music into the manager.
        """
        try:
            pg.mixer.music.load(filepath)
            self.music_loaded = True
        except pg.error as e:
            print(f"Error loading music {filepath}: {e}")

    def play_music(self, loops=-1):
        """
        Play background music. Specify how many times to loop (-1 for infinite).
        """
        if self.music_loaded:
            pg.mixer.music.play(loops=loops)
        else:
            print("No music loaded!")

    def pause_music(self):
        """
        Pause the currently playing music.
        """
        pg.mixer.music.pause()

    def unpause_music(self):
        """
        Unpause the currently paused music.
        """
        pg.mixer.music.unpause()

    def stop_music(self):
        """
        Stop the background music.
        """
        pg.mixer.music.stop()

    def set_music_volume(self, volume):
        """
        Set the volume of the background music (0.0 to 1.0).
        """
        pg.mixer.music.set_volume(volume)

    def fadeout_music(self, time):
        """
        Fade out the background music over a specified time (in milliseconds).
        """
        pg.mixer.music.fadeout(time)
