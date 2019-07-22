import rhinoscriptsyntax as rs

BITS = 16
def interpret(x):
	output = []
	is_negative = False

	if number < 0:
		is_negative = True
		number = -1 * number

	binary = bin(number)[2:].zfill(BITS-1)

	for i in range(BITS-1):
		output[i] = int(binary[i])

	if is_negative:
		output[BITS] = 0
	else:
		output[BITS] = 1

	return output