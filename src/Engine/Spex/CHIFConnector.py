from Spex.src.main import SpexInterpreter

class SpexConnector:
    def __init__(self, Engine):
        self.Engine = Engine
        self.interpreter = SpexInterpreter()
    def load_object(self, name):
        print(f"Object '{name}' loaded.")

    def load_texture(self, name):
        print(f"Texture '{name}' loaded.")

    def set_physics(self, enabled):
        print(f"Physics {'enabled' if enabled else 'disabled'}.")

    def set_cutscene_mode(self, enabled):
        print(f"Cutscene mode {'enabled' if enabled else 'disabled'}.")

    def set_camera_collision(self, enabled):
        print(f"Camera collision {'enabled' if enabled else 'disabled'}.")
    def execute_script(path = None, file = None):
        if path:
            pass
        if file:
            pass
        if not path and file:
            print("Spex: No Script to Execute")
            return