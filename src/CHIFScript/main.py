from time import time

class CHIFScriptInterpreter:
    def __init__(self, engine):
        self.engine_connector = engine
        self.current_loaded_objects = {}
        self.active_variables = {}
        self.active_functions = {}
        self.current_line = 0
        self.commands = {
            'load': self.load_resource,
            'version': self.check_version,
            'physics': self.set_physics,
            'cutscene': self.set_cutscene_mode,
            'camera': self.set_camera_collision,
            'decl': self.declare_resource,
            'destroy': self.destroy_resource,
            'access': self.access_resource,
            'declf': self.declare_function,
            'Sound': self.handle_sound,
            'print': self.print_func
        }

    def execute(self, script):
        beginning = time()
        script = self.remove_comments(script)
        script = self.handle_function_declarations(script)  # Process function declarations separately
        commands = script.split(';')
        for command in commands:
            self.current_line += 1
            command = command.strip()
            if not command:
                continue
            command_parts = command.split()
            if not command_parts:
                continue
            cmd = command_parts[0].lower()
            if cmd in self.commands:
                self.commands[cmd](command_parts)
            elif cmd in self.active_functions:
                self.execute_function(cmd)  # Execute function if it is called
            else:
                print(f"Error - line::{self.current_line}: Unknown command: {command}")
        print(f"Executed Code in {time() - beginning:.2f} seconds")

    def execute_function(self, function_name):
        function_code = self.active_functions.get(function_name)
        if not function_code:
            print(f"Error: Function '{function_name}' not found.")
            return

        # Replace variables in the function code
        function_code = self.replace_variables_in_code(function_code)

        # Execute the function code
        try:
            exec(function_code)
        except Exception as e:
            print(f"Error executing function '{function_name}': {e}")

    def replace_variables_in_code(self, code):
        # Replace variables in code if any are used
        for var_name, var_value in self.active_variables.items():
            code = code.replace(f"${{{var_name}}}", str(var_value))
        return code

    def remove_comments(self, script):
        lines = script.split('\n')
        script_lines = [line.split('//')[0].strip() for line in lines]
        return '\n'.join(script_lines)

    def handle_function_declarations(self, script):
        import re
        # Match all declf function declarations with code blocks
        pattern = re.compile(r'declf\s+(\w+)\s*\{(.*?)\}', re.DOTALL)
        matches = pattern.findall(script)
        for function_name, function_code in matches:
            function_code = function_code.strip()
            if function_code.startswith('{'):
                function_code = function_code[1:].strip()
            if function_code.endswith('}'):
                function_code = function_code[:-1].strip()
            self.active_functions[function_name] = function_code
            print(f"Declared function '{function_name}' with code: '{function_code}'")
        # Remove the function declarations from the script
        script = pattern.sub('', script)
        return script

    def check_version(self, command_parts):
        if len(command_parts) != 2:
            print(f"Error: Invalid VERSION command format.")
            return
        version_code = command_parts[1]
        current_version = "0.1"
        if version_code != current_version:
            print(f"Error: Unknown or older Script Version: {version_code} -> {current_version}")
        print(f"Using Version {version_code}")

    def load_resource(self, command_parts):
        if len(command_parts) != 3:
            print(f"Error: Invalid load command format: '{' '.join(command_parts)}'")
            return
        resource_type = command_parts[1].lower()
        resource_name = command_parts[2]
        if resource_type == 'object':
            if self.check_if_variable(resource_name):
                resource_name = self.access_variable(resource_name)
            print(f"Loading object: {resource_name}")
            self.engine_connector.load_object(resource_name)
        elif resource_type == 'texture':
            print(f"Loading texture: {resource_name}")
            self.engine_connector.load_texture(resource_name)
        else:
            print(f"Error: Unknown resource type: {resource_type}")

    def set_physics(self, command_parts):
        if len(command_parts) != 2:
            print(f"Error: Invalid physics command format: '{' '.join(command_parts)}'")
            return
        state = command_parts[1].lower()
        if state not in ['on', 'off']:
            print(f"Error: Invalid physics state: {state}")
            return
        print(f"Setting physics: {state}")
        self.engine_connector.set_physics(state == 'on')

    def set_cutscene_mode(self, command_parts):
        if len(command_parts) != 2:
            print(f"Error: Invalid cutscene command format: '{' '.join(command_parts)}'")
            return
        state = command_parts[1].lower()
        if state not in ['on', 'off']:
            print(f"Error: Invalid cutscene mode state: {state}")
            return
        print(f"Setting cutscene mode: {state}")
        self.engine_connector.set_cutscene_mode(state == 'on')

    def set_camera_collision(self, command_parts):
        if len(command_parts) != 2:
            print(f"Error: Invalid camera command format: '{' '.join(command_parts)}'")
            return
        state = command_parts[1].lower()
        if state not in ['on', 'off']:
            print(f"Error: Invalid camera collision state: {state}")
            return
        print(f"Setting camera collision: {state}")
        self.engine_connector.set_camera_collision(state == 'on')

    def declare_resource(self, command_parts):
        if len(command_parts) != 4:
            print(f"Error: Invalid declaration command format: '{' '.join(command_parts)}'")
            return
        resource_type = command_parts[1].lower()
        resource_name = command_parts[3]
        identifier = command_parts[2]
        if resource_type == 'object':
            self.current_loaded_objects[resource_name] = identifier
            print(f"Declared Object '{identifier}' as '{resource_name}'")
        else:
            print(f"Error: Unknown declaration type: {resource_type}")

    def destroy_resource(self, command_parts):
        if len(command_parts) != 3:
            print(f"Error: Invalid destroy command format: '{' '.join(command_parts)}'")
            return
        resource_type = command_parts[1].lower()
        resource_name = command_parts[2]
        if resource_type == 'object':
            if resource_name in self.current_loaded_objects:
                deleted_value = self.current_loaded_objects.pop(resource_name)
                print(f"Deleted Object '{resource_name}' with value '{deleted_value}'")
            else:
                print(f"Error: Object '{resource_name}' not found")
        else:
            print(f"Error: Unknown resource type: {resource_type}")

    def access_resource(self, command_parts):
        if len(command_parts) != 3:
            print(f"Error: Invalid access command format: '{' '.join(command_parts)}'")
            return
        resource_type = command_parts[1].lower()
        resource_name = command_parts[2]
        if resource_type == 'object':
            if resource_name in self.current_loaded_objects:
                print(f"Accessed '{resource_name}' with '{self.current_loaded_objects[resource_name]}'")
            else:
                print(f"Error: Resource '{resource_name}' cannot be accessed")
        else:
            print(f"Error: Unknown resource type: {resource_type}")

    def check_if_variable(self, string):
        if string.startswith("$"):
            resource_name = string[1:]
            return resource_name in self.current_loaded_objects
        return False

    def access_variable(self, resource_name):
        if resource_name.startswith('$'):
            resource_name = resource_name[1:]
            if resource_name in self.current_loaded_objects:
                return self.current_loaded_objects[resource_name]
            else:
                print(f"Error: Variable '{resource_name}' not declared")
                exit(1)
        else:
            print(f"Error: Invalid variable format")
            exit(1)

    def declare_function(self, command_parts):
        if len(command_parts) < 3:
            print(f"Error: Invalid function declaration format: '{' '.join(command_parts)}'")
            return

        function_name = command_parts[1]
        function_code = ' '.join(command_parts[2:]).strip()
        if function_code.startswith('{') and function_code.endswith('}'):
            function_code = function_code[1:-1].strip()
        self.active_functions[function_name] = function_code
        print(f"Declared function '{function_name}' with code: '{function_code}'")
    def handle_sound(self,command_parts):
        if len(command_parts) < 2:
            print(f"Error - line{self.current_line}: Invalid sound function format: '{' '.join(command_parts)}'")
            return
        functionc = command_parts[1]
        if functionc == "play":
            variablec = command_parts[2]
            if self.check_if_variable(variablec):
                variablec = self.access_variable(variablec)
                print(f"Playing Sound {variablec}")
                return
            else: print(f"Playing Sound {variablec}")
        else: return
    def print_func(self,command_parts):
        if len(command_parts) < 2:
            print(f"Error - line{self.current_line}: Invalid print function format: '{' '.join(command_parts)}'")
            return 
        string_table = command_parts[1:]
        printed_string =""
        for stringf in string_table:
            dummy = stringf
            if self.check_if_variable(dummy):
                dummy = self.access_variable(dummy)
                if printed_string == "":
                    printed_string = printed_string + str(dummy)
                else:   
                    printed_string = printed_string + " " + str(dummy)
            else:
                if printed_string == "":
                    printed_string = printed_string + str(dummy)
                else:   
                    printed_string = printed_string + " " + str(dummy)
        print(printed_string)
# Example usage:

class MockEngineConnector:
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

# Example script
script = """
decl object 
"""

# Create interpreter with a mock engineconnector
engine = MockEngineConnector()
interpreter = CHIFScriptInterpreter(engine)
interpreter.execute(script)
