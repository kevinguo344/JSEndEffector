"""Provides a scripting component.
    Inputs:
        x: The x script variable
    Output:
        a: The a output variable"""

__author__ = "user"
__version__ = "2019.07.23"

BITS = 16
def interpret(number):
	output = [None] * BITS
	is_negative = False

	if number < 0:
		is_negative = True
		number = abs(number)

	binary = bin(number)[2:].zfill(BITS-1)

	for i in range(BITS-2,-1,-1):
		if int(binary[i]) == 1:
			output[BITS-2-i] = True
		else:
			output[BITS-2-i] = False
	if is_negative:
		output[BITS-1] = False
	else:
		output[BITS-1] = True
	return output

a = interpret(int(x))
