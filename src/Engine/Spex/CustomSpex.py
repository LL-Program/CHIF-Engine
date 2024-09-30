from Spex.src.main import SpexInterpreter

class SpexEngineInterpreter(SpexInterpreter):
    def __init__(self,EngineConnector):
        self.EngineObjects = {
            'Scene', self.create_SceneObject
            }
        self.commands = self.commands + self.EngineObjects
        self.ActiveScenes = []
    def create_SceneObject(self, command_parts):
        if len(command_parts) < 2:
            print(f"Error - line {self.current_line}: Invalid Object decleration command format.")
        scene_name = command_parts[1]
        self.ActiveScenes.append(scene_name)
        print(self.ActiveScenes)
inter = SpexEngineInterpreter("x")
inter.execute("Scene new_scene;")