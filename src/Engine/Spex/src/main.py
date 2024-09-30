from Spex.src.maths import *
from time import time
import os
from math import *
from Spex.src.datahandling import DataSystem
class SpexInterpreter:
    def __init__(self):
        #Systems that work together
        self.Datasystem = DataSystem(self)
        self.active_variables = {}
        self.active_functions = {}
        self.current_line = 0
        self.commands = {
            'version': self.check_version,
            'decl': self.declare_resource,
            'destroy': self.destroy_resource,
            'access': self.access_resource,
            'declf': self.declare_function,
            'print': self.print_func,
            'explode': self.explode_command,
            'math': self.execute_math,
            'include': self.include_command,
            'if': self.if_command,
            'pyfunc' : self.pyfunc_command
        }
    def execute(self, script,child = None):
        beginning = time()
        script = self.remove_comments(script)
        script = self.handle_function_declarations(script)
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
                self.execute_function(cmd)
            elif self.is_valid_changable_syntax(cmd):
                self.excute_changable(cmd)
            else:
                print(f"Error - line::{self.current_line}: Unknown command: {command}")
        if child:
            pass
        else:
            print(f"Executed Code in {time() - beginning} seconds")

    def execute_math(self, command_parts):
        if len(command_parts) < 3:
            print(f"Error - line {self.current_line}: Invalid math command format.")
            return

        variable_name = command_parts[1]
        expression = ' '.join(command_parts[2:])

        expression = self.replace_variables_in_expression(expression)
        tokens = tokenize_math_expression(expression)
        postfix_expr = infix_to_postfix(tokens)
        result = evaluate_postfix(postfix_expr)

        if result is not None:
            self.Datasystem.memory[variable_name]["data"] = result
            self.Datasystem.memory[variable_name]["dtype"] = "float"
        else:
            print(f"Error - line {self.current_line}: Invalid expression: {expression}")

    def replace_variables_in_expression(self, expression):
        for var_name, var_info in self.Datasystem.memory.items():
            if "data" in var_info:
                expression = expression.replace(f"${var_name}", str(var_info["data"]))
        return expression



    def handle_math(self, command_parts):
        if len(command_parts) < 4:
            print(f"Error: Invalid math command format.")
            return

        variable_name = command_parts[1]  # result1, result2, etc.
        expression = ' '.join(command_parts[2:])
        expression = self.replace_variables_in_expression(expression)

        try:
            result = eval(expression)
            self.Datasystem.memory[variable_name]["data"] = result
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
                operator_stack.pop()

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
        pattern = re.compile(r'declf\s+(\w+)\s*\{(.*?)\}', re.DOTALL)
        matches = pattern.findall(script)
        for function_name, function_code in matches:
            function_code = function_code.strip()
            self.active_functions[function_name] = function_code.strip()
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
        dtype = command_parts[1]
        resource_name = command_parts[3]
        variable_name = command_parts[2]
        if dtype in self.Datasystem.datatypes:
            if variable_name not in self.Datasystem.memory:
                self.Datasystem.register_Data(variable_name,dtype,resource_name,isthere=True)
            self.Datasystem.register_Data(variable_name,dtype,resource_name)
            return
        else:
            print(f"Error: Invalid Datatype Object: '{' '.join(command_parts)}'")
            return

    def destroy_resource(self, command_parts):
        if len(command_parts) != 2:
            print(f"Error: Invalid destroy command format: '{' '.join(command_parts)}'")
            return
        variable_name = command_parts[1]
        if variable_name in self.Datasystem.memory:
            del self.Datasystem.memory[variable_name]
        else:
            print(f"Error - line::{self.current_line}: Variable '{variable_name}' not found.")

    def check_if_variable(self, var_name):
        return var_name.startswith('$') and var_name[1:] in self.Datasystem.memory

    def access_variable(self, var_name):
        return self.Datasystem.memory[var_name[1:]]["data"]

    def declare_function(self, command_parts, script):
        function_name = command_parts[1]
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
        function_code = '; '.join(function_body).strip()
        self.active_functions[function_name] = function_code
        #print(f"Declared function '{function_name}' with code: {function_code}")

    def access_resource(self, command_parts):
        if len(command_parts) != 3:
            print(f"Error: Invalid access command format: '{' '.join(command_parts)}'")
            return
        resource_type = command_parts[1].lower()
        resource_name = command_parts[2]
        if resource_type == 'object':
            if resource_name in self.Datasystem.memory:
                print(f"Accessed Dtype ('{self.Datasystem.memory[resource_name]["dtype"]}') '{resource_name}' with Value '{self.Datasystem.memory[resource_name]["data"]}'")
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
        self.Datasystem.memory.clear()
    def execute_function(self, function_name):
        function_code = self.active_functions.get(function_name)
    
        if not function_code:
            print(f"Error: Function '{function_name}' not found.")
            return

        function_code = self.replace_variables_in_expression(function_code)
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
    def include_command(self, command_parts):
        if len(command_parts) != 2:
            print(f"Error - line::{self.current_line}: Invalid include command format: '{' '.join(command_parts)}'")
            return

        filename = command_parts[1]
        included_code = self.load_file(filename)
        if included_code:
            #print(f"Including {filename}...")
            self.handle_function_declarations(included_code)
            self.execute(included_code,child=True)
        else:
            print(f"Failed to include file: {filename}")
    import os

    def load_file(self, filename,childlevel = None,suffix = None):
        levels = 2
        name_suffix = ".sx"
        if childlevel:
            levels = childlevel
        if suffix:
            name_suffix = suffix
        current_dir = os.getcwd()
        filename = filename + name_suffix
        path = os.path.join(current_dir, filename)
        #print(f"Checking for file at: {path}")
        if os.path.isfile(path):
            with open(path, 'r') as file:
                return file.read()
        directories_to_check = [current_dir]

        for level in range(levels):
            next_directories = []
            for directory in directories_to_check:
                try:
                    items = os.listdir(directory)
                except PermissionError:
                    continue
                for item in items:
                    item_path = os.path.join(directory, item)
                    if os.path.isdir(item_path):
                        next_directories.append(item_path)
                        file_path = os.path.join(item_path, filename)
                        #print(f"Checking for file at: {file_path}")
                        if os.path.isfile(file_path):
                            with open(file_path, 'r') as file:
                                return file.read()
            directories_to_check = next_directories
        print(f"Error: File '{filename}' not found in any of the searched directories.")
        return None
    def if_command(self, command_parts):
        if len(command_parts) >= 2:
            print(f"Error - line::{self.current_line}: Invalid include command format: '{' '.join(command_parts)}'")
            return
        for command in command_parts:
            self.replace_variables_in_expression(command_parts)
        var1 = command_parts[1]
        var2 = command_parts[3]
        token = command_parts[2]
        if token == "is":
            var1 = self.replace_variables_in_expression(var1)
            var2 = self.replace_variables_in_expression(var2)
    def pyfunc_command(self,command_parts):
        if not (len(command_parts) >= 2):
            print(f"Error - line::{self.current_line}: Invalid pyfunc command format: '{' '.join(command_parts)}'")
            return
        func_name = command_parts[1]
        if len(command_parts) > 2:
            func_args = command_parts[2:]
            dummy_arg = []
            for arg in func_args:
                dummy_arg.append(self.replace_variables_in_expression(arg))
            func_args = dummy_arg
            strs = ""
            for args in func_args:
                if args == func_args[-1]:
                    strs = strs + args
                else:
                    strs = strs + args + ","
            func_args = strs
        else:
            func_args = ""
        eval(func_name+'('+func_args+')')
    def is_valid_changable_syntax(self,line):
        pattern = r"\$\w+<\w+;"
        if re.fullmatch(pattern, line):
            return True
        return False
    def excute_changable(self,changable):
        commands = changable.strip("<")
        factor = commands[0]
        if self.check_if_variable(factor):
            factor = self.replace_variables_in_expression(factor)
        

# # Example script
# script = """
# decl float h 0;
# math h 5+5;
# print 5 + 5 = $h;
# """
# #interpreter with connector
# interpreter = SpexInterpreter()
# interpreter.execute(script)
