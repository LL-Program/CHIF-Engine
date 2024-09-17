import re
from collections import deque

# Helper function to determine operator precedence
def precedence(op):
    if op in ('+', '-'):
        return 1
    if op in ('*', '/'):
        return 2
    return 0

# Shunting Yard Algorithm to convert infix expression (tokenized) to postfix (RPN)
def infix_to_postfix(tokens):
    output = []
    operators = deque()

    for token_type, token_value in tokens:
        if token_type == 'NUMBER':
            output.append(token_value)
        elif token_type == 'OPERATOR':
            while operators and precedence(operators[-1]) >= precedence(token_value):
                output.append(operators.pop())
            operators.append(token_value)
        elif token_value == '(':
            operators.append(token_value)
        elif token_value == ')':
            while operators and operators[-1] != '(':
                output.append(operators.pop())
            operators.pop()  # Remove '(' from stack

    while operators:
        output.append(operators.pop())

    return output
# Function to evaluate a postfix expression
def evaluate_postfix(expression):
    stack = []

    for token in expression:
        if isinstance(token, float):  # If it's a number, push it onto the stack
            stack.append(token)
        else:
            right = stack.pop()
            left = stack.pop()
            if token == '+':
                stack.append(left + right)
            elif token == '-':
                stack.append(left - right)
            elif token == '*':
                stack.append(left * right)
            elif token == '/':
                stack.append(left / right)

    return stack[0] if stack else None

def tokenize_math_expression(expression):
    # Tokenize the mathematical expression into numbers, operators, and parentheses
    tokens = []
    operators = ['+', '-', '/', '*', '(', ')']

    # Adjust regex to handle floats and integers
    for token in re.findall(r'\d+\.\d+|\d+|\+|\-|\*|\/|\(|\)', expression):
        if re.match(r'\d+\.\d+|\d+', token):  # Match both float and integer
            tokens.append(('NUMBER', float(token)))  # Convert to float for both integer and float numbers
        elif token in operators:
            tokens.append(('OPERATOR' if token in ['+', '-', '*', '/'] else 'PAREN', token))
        else:
            pass  # Ignore invalid tokens

    return tokens
