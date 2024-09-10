#TODO:  Verschiedene Funktionen für Fenster (1 : Grafikeinstellungen, 2 : Objekte)
import customtkinter
from UI.GraphicsWindow import GraphicsSettings
customtkinter.set_appearance_mode("System")  # Modes: system (default), light, dark
customtkinter.set_default_color_theme("blue")  # Themes: blue (default), dark-blue, green



class UIManager:
    def __init__(self,Engine):
        self.engine = Engine
        self.GraphicsSettings = None
    def openGraphicsSettings(self):
        self.GraphicsSettings = GraphicsSettings(self.engine)