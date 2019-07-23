"""Provides a scripting component.
    Inputs:
        x: The x script variable
    Output:
        a: The a output variable"""

__author__ = "Kevin Guo"
__version__ = "2019.07.23"

BITS = 16
def interpretChunks(number):
	# sets up default values of variables
	num_chunks = 1
	is_negative = False

	chunks = []
	output = []
	num_chunks = []

	# splits the number into chunks
	if number > 32767:
		num_chunks = int(number / 32767)
		for i in range(num_chunks):
			chunks.append(32767)
		if number % 32767 > 0:
			chunks.append(number % 32767)
			num_chunks += 1
	elif number < -32768:
		num_chunks = int(number / -32768)
		for i in range(num_chunks):
			chunks.append(-32768)
		if number % -32768 < 0:
			chunks.append(number % -32768)
			num_chunks += 1

	for i in range(num_chunks):
		output.append(interpret(chunks[i]))

	return output

def interpret(num):
	if num < 0:
		is_negative = True
		num = abs(num)

	binary = bin(num)[2:].zfill(BITS-1)

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
