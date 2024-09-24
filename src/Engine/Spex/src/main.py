from Spex.src.maths import *
from time import time
import re

class SpexInterpreter:
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
            'print': self.print_func,
            'explode': self.explode_command,
            'math': self.execute_math,

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
        print(f"Executed Code in {time() - beginning} seconds")

    def execute_math(self, command_parts):
        if len(command_parts) < 3:
            print(f"Error - line {self.current_line}: Invalid math command format.")
            return

        variable_name = command_parts[1]
        expression = ' '.join(command_parts[2:])

        # Tokenize and evaluate the mathematical expression
        expression = self.replace_variables_in_expression(expression)
        tokens = tokenize_math_expression(expression)
        postfix_expr = infix_to_postfix(tokens)
        result = evaluate_postfix(postfix_expr)

        if result is not None:
            self.current_loaded_objects[variable_name] = result
        else:
            print(f"Error - line {self.current_line}: Invalid expression: {expression}")

    def replace_variables_in_expression(self, expression):
        # Replace all variables (e.g., $var) in the expression with their actual values
        for var_name, var_value in self.current_loaded_objects.items():
            expression = expression.replace(f"${var_name}", str(var_value))
        return expression


    def handle_math(self, command_parts):
        if len(command_parts) < 4:
            print(f"Error: Invalid math command format.")
            return

        variable_name = command_parts[1]  # result1, result2, etc.
        expression = ' '.join(command_parts[2:])  # The full expression like "5+10" or "$result1*2"
    
        # Replace variables in the expression (e.g., $result1) with their actual values
        expression = self.replace_variables_in_expression(expression)

        try:
            # Evaluate the mathematical expression safely
            result = eval(expression)
            # Store the result in current_loaded_objects
            self.current_loaded_objects[variable_name] = result
            #print(f"Stored {variable_name} = {result}")
        except Exception as e:
            print(f"Error evaluating expression '{expression}': {e}")

    def infix_to_postfix(self, tokens):
        """Convert infix token list to postfix (RPN) using the shunting-yard algorithm."""
        precedence = {'+': 1, '-': 1, '*': 2, '/': 2}
        output = []
        operator_stack = []

        for token_type, token_value in tokens:
            if token_type == 'NUMBER':
                output.append((token_type, token_value))
            elif token_type == 'OPERATOR':
                while (operator_stack and operator_stack[-1][0] == 'OPERATOR' and
                       precedence[operator_stack[-1][1]] >= precedence[token_value]):
                    output.append(operator_stack.pop())
                operator_stack.append((token_type, token_value))
            elif token_value == '(':
                operator_stack.append((token_type, token_value))
            elif token_value == ')':
                while operator_stack and operator_stack[-1][1] != '(':
                    output.append(operator_stack.pop())
                operator_stack.pop()  # Pop the '(' from the stack

        while operator_stack:
            output.append(operator_stack.pop())

        return output

    def evaluate_postfix_expression(self, tokens):
        """Evaluate a postfix expression from a list of tokens."""
        stack = []

        for token_type, token_value in tokens:
            if token_type == 'NUMBER':
                stack.append(token_value)
            elif token_type == 'OPERATOR':
                b = stack.pop()
                a = stack.pop()
                if token_value == '+':
                    stack.append(a + b)
                elif token_value == '-':
                    stack.append(a - b)
                elif token_value == '*':
                    stack.append(a * b)
                elif token_value == '/':
                    if b == 0:
                        print(f"Error - line::{self.current_line}: Division by zero.")
                        return None
                    stack.append(a / b)
            else:
                print(f"Error - line::{self.current_line}: Unknown token {token_value}")
                return None

        return stack[0] if stack else None

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

    def load_resource(self, command_parts):
        if len(command_parts) != 3:
            print(f"Error: Invalid load command format: '{' '.join(command_parts)}'")
            return
        resource_type = command_parts[1].lower()
        resource_name = command_parts[2]
        if resource_type == 'object':
            if self.check_if_variable(resource_name):
                resource_name = self.access_variable(resource_name)
            self.engine_connector.load_object(resource_name)
        elif resource_type == 'texture':
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
        self.engine_connector.set_physics(state == 'on')

    def set_cutscene_mode(self, command_parts):
        if len(command_parts) != 2:
            print(f"Error: Invalid cutscene command format: '{' '.join(command_parts)}'")
            return
        state = command_parts[1].lower()
        if state not in ['on', 'off']:
            print(f"Error: Invalid cutscene mode state: {state}")
            return
        self.engine_connector.set_cutscene_mode(state == 'on')

    def set_camera_collision(self, command_parts):
        if len(command_parts) != 2:
            print(f"Error: Invalid camera command format: '{' '.join(command_parts)}'")
            return
        state = command_parts[1].lower()
        if state not in ['on', 'off']:
            print(f"Error: Invalid camera collision state: {state}")
            return
        self.engine_connector.set_camera_collision(state == 'on')

    def declare_resource(self, command_parts):
        if len(command_parts) != 4:
            print(f"Error: Invalid declaration command format: '{' '.join(command_parts)}'")
            return
        resource_type = command_parts[1].lower()
        resource_name = command_parts[2]
        variable_name = command_parts[3]
        self.current_loaded_objects[variable_name] = resource_name

    def destroy_resource(self, command_parts):
        if len(command_parts) != 2:
            print(f"Error: Invalid destroy command format: '{' '.join(command_parts)}'")
            return
        variable_name = command_parts[1]
        if variable_name in self.current_loaded_objects:
            del self.current_loaded_objects[variable_name]
        else:
            print(f"Error - line::{self.current_line}: Variable '{variable_name}' not found.")

    def check_if_variable(self, var_name):
        return var_name.startswith('$') and var_name[1:] in self.current_loaded_objects

    def access_variable(self, var_name):
        return self.current_loaded_objects.get(var_name[1:], None)

    def declare_function(self, command_parts, script):
        # command_parts[1] is the function name
        function_name = command_parts[1]
    
        # Collect all lines between '{' and '}'
        function_body = []
        inside_function = False
        for line in script.splitlines():
            line = line.strip()
        
            if line.startswith("{"):
                inside_function = True
                continue
        
            if line.endswith("}"):
                inside_function = False
                break
        
            if inside_function:
                function_body.append(line)

        # Join the function body back into a single string, delimited by semicolons
        function_code = '; '.join(function_body).strip()
    
        # Store the function code in active_functions
        self.active_functions[function_name] = function_code
        #print(f"Declared function '{function_name}' with code: {function_code}")

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
    def handle_sound(self, command_parts):
        if len(command_parts) < 2:
            print(f"Error - line::{self.current_line}: Invalid sound function format: '{' '.join(command_parts)}'")
            return
        functionc = command_parts[1]
        if functionc == "play":
            variablec = command_parts[2]
            if self.check_if_variable(variablec):
                variablec = self.access_variable(variablec)

    def print_func(self, command_parts):
        if len(command_parts) < 2:
            print(f"Error - line::{self.current_line}: Invalid print function format: '{' '.join(command_parts)}'")
            return 
        string_table = command_parts[1:]
        printed_string = ""
        for stringf in string_table:
            dummy = stringf
            if self.check_if_variable(dummy):
                dummy = self.access_variable(dummy)
                printed_string += f"{dummy} "
            else:
                printed_string += f"{dummy} "
        print(printed_string.strip())

    def explode_command(self, command_parts):
        if len(command_parts) > 1:
            print(f"Error - line::{self.current_line}: Invalid explosion function format: '{' '.join(command_parts)}'")
            return
        self.current_loaded_objects.clear()
    def execute_function(self, function_name):
        # Check if the function exists in active_functions
        function_code = self.active_functions.get(function_name)
    
        if not function_code:
            print(f"Error: Function '{function_name}' not found.")
            return

        # Replace variables in the function code
        function_code = self.replace_variables_in_expression(function_code)

        # Split the function code into lines or commands and execute each
        commands = function_code.split(';')
        for command in commands:
            command = command.strip()
            if not command:
                continue
            command_parts = command.split()
            cmd = command_parts[0].lower()

            if cmd in self.commands:
                self.commands[cmd](command_parts)
            else:
                print(f"Error in function '{function_name}': Unknown command '{cmd}'")


# Example usage
class EngineConnector:
    def __init__(self, Engine):
        self.Engine = Engine

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
declf print_pi { // Deklariere   Funktion
    math variable (5+1)/2;
    print $variable;
}

"""

# Create interpreter with engine connector
engine = EngineConnector("Engine Here")
interpreter = SpexInterpreter(engine)
interpreter.execute(script)
