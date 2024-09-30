import re
from collections import deque
from math import *
# Helper function
def precedence(op):
    if op in ('+', '-'):
        return 1
    if op in ('*', '/'):
        return 2
    return 0

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
            operators.pop()

    while operators:
        output.append(operators.pop())

    return output

def evaluate_postfix(expression):
    stack = []

    for token in expression:
        if isinstance(token, float):
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
    tokens = []
    operators = ['+', '-', '/', '*', '(', ')']

    for token in re.findall(r'\d+\.\d+|\d+|\+|\-|\*|\/|\(|\)', expression):
        if re.match(r'\d+\.\d+|\d+', token):
            tokens.append(('NUMBER', float(token))) 
        elif token in operators:
            tokens.append(('OPERATOR' if token in ['+', '-', '*', '/'] else 'PAREN', token))
        else:
            pass

    return tokens
def pi_key_identifier() -> float:
    return pi()