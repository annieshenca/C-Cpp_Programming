###############################################################################
# Annie Shen (ashen7)
# CMPS 109 Harrison - Spring 2018
# Lab 1 - Python3 Reverse Polish Notation (RPN) Calculator
# Due Apr 8th, 2018
###############################################################################
#
# Accepts an arbitrary number of arguments in RPN, evaluates them and prints
# the answer to stdout.

import sys
import shlex
# shlex from: https://stackoverflow.com/questions/899276/python-how-to-parse-strings-to-look-like-sys-argv
test = shlex.split(sys.argv[1])
stack = []

# enumerate from: https://stackoverflow.com/questions/522563/accessing-the-index-in-python-for-loops
for idx, oper in enumerate(test):
    oper = str(oper)

    if oper.isdigit():
        stack.append(oper)

    else:
        # Once reach an operator, pop the newest two numbers on stack, perform
        # the operation with the given operator, and push result back onto stack.
        num2 = int(stack.pop())
        num1 = int(stack.pop())

        if oper is '+':
            result = num1 + num2
        elif oper is '-':
            result = num1 - num2
        elif oper is '*':
            result = num1 * num2
        elif oper is '/':
            result = num1/num2
        elif oper is '^':
            result = num1**num2

        stack.append(result)

# End of operations, return the result, which should be the only item in stack.
print(stack[0])
