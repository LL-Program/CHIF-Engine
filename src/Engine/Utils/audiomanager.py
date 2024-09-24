import pygame as pg

class AudioManager:
    def __init__(self,app):
        pg.mixer.init()
        self.sounds = {}
        self.music_loaded = False

    def load_sound(self, name, filepath):
        try:
            sound = pg.mixer.Sound(filepath)
            self.sounds[name] = sound
        except pg.error as e:
            print(f"Error loading sound {filepath}: {e}")

    def play_sound(self, name, loops=0):
        if name in self.sounds:
            self.sounds[name].play(loops=loops)
        else:
            print(f"Sound '{name}' not found!")

    def stop_sound(self, name):
        if name in self.sounds:
            self.sounds[name].stop()

    def set_sound_volume(self, name, volume):
        if name in self.sounds:
            self.sounds[name].set_volume(volume)

    def load_music(self, filepath):
        try:
            pg.mixer.music.load(filepath)
            self.music_loaded = True
        except pg.error as e:
            print(f"Error loading music {filepath}: {e}")

    def play_music(self, loops=-1):
        if self.music_loaded:
            pg.mixer.music.play(loops=loops)
        else:
            print("No music loaded!")

    def pause_music(self):
        pg.mixer.music.pause()

    def unpause_music(self):
        pg.mixer.music.unpause()

    def stop_music(self):
        pg.mixer.music.stop()

    def set_music_volume(self, volume):
        pg.mixer.music.set_volume(volume)

    def fadeout_music(self, time):
        pg.mixer.music.fadeout(time)
