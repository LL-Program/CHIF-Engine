import pygame as pg
import sys

class InputManager:
    def __init__(self):
        self.mouse_buttons = {}
        self.mouse_button_down = False
        self.mouse_button_up = False
    def get_mouse_position(self):
        return pg.mouse.get_pos()
    def update(self):
        self.mouse_button_down = pg.mouse.get_pressed()[0]
        self.mouse_button_up = not self.mouse_button_down
        for event in pg.event.get():
            if event.type == pg.QUIT:
                pg.quit()
                sys.exit()
            if event.type == pg.KEYDOWN and event.key == pg.K_ESCAPE:
                pg.quit()
                sys.exit()
            if event.type == pg.MOUSEBUTTONDOWN:
                self.mouse_button_down = True
                self.mouse_buttons[event.button] = True
            if event.type == pg.MOUSEBUTTONUP:
                self.mouse_button_up = True
                self.mouse_buttons[event.button] = False