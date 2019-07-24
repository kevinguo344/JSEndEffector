"""Provides a scripting component.
    Inputs:
        x: The integer input variable
    Output:
        bool_list: The boolean output variable
        num_chunks: The integer output variable"""

__author__ = "Kevin Guo"
__version__ = "2019.07.24"

BITS = 16
def interpretChunks(number):
	# sets up default values of variables
	chunk_amount = 1
	chunks = []

	# splits the number into chunks
	if number > 32767:
		chunk_amount = int(number / 32767)
		for i in range(chunk_amount):
			chunks.append(32767)
		if number % 32767 > 0:
			chunks.append(number % 32767)
			chunk_amount += 1
	elif number < -32768:
		chunk_amount = int(number / -32768)
		for i in range(chunk_amount):
			chunks.append(-32768)
		if number % -32768 < 0:
			chunks.append(number % -32768)
			chunk_amount += 1
	else:
		chunks.append(number)

	output = []

	for i in range(chunk_amount):
		output.extend(interpret(chunks[i]))

	print(output)
	return output,chunk_amount

def interpret(num):
	out = [None] * BITS
	is_negative = False

	if num < 0:
		is_negative = True
		num = abs(num)

	binary = bin(num)[2:].zfill(BITS-1)

	for i in range(BITS-2,-1,-1):
		if int(binary[i]) == 1:
			out[BITS-2-i] = True
		else:
			out[BITS-2-i] = False
	if is_negative:
		out[BITS-1] = False
	else:
		out[BITS-1] = True
	return out

bool_list,num_chunks = interpretChunks(int(x))
