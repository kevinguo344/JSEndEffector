"""Provides a scripting component.
    Inputs:
        x: The x script variable
        y: The y script variable
    Output:
        a: The a output variable"""

__author__ = "user"
__version__ = "2019.07.22"

BITS = 16
def interpret(number):
	output = [None] * BITS
	is_negative = False

	if number < 0:
		is_negative = True
		number = abs(number)

	binary = bin(number)[2:].zfill(BITS-1)

	for i in range(BITS-1):
		output[i] = int(binary[i])

	if is_negative:
		output[BITS-1] = 0
	else:
		output[BITS-1] = 1

	return output

a = interpret(int(x))
