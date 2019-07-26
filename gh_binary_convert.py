"""Provides a scripting component.
    Inputs:
        steps: Steps motor will take
        pulse_width: Pulse width of each step (in microseconds)
    Output:
        bool_list: List of on/off commands for the 16 pins
        num_chunks: Number of times a command is sent out to the 16 pins"""

__author__ = "Kevin Guo"
__version__ = "2019.07.26"

BITS = 16
DELIMITER = -1
OFF = 0

#assumes pulse width will never overflow

def createCommands(stps,pulse):
    chunk_number = 0
    final_outputs = []

    cmd_list_steps,num_chunks_steps = interpretChunks(stps)
    cmd_list_delimeter = interpret(DELIMITER)
    cmd_list_pulse = interpret(pulse)
    cmd_list_off = interpret(OFF)\

    for i in range(num_chunks_steps):
        # adds step commands to outputs
        final_outputs.extend(cmd_list_steps[16*i:16*(i+1)])
        # adds delimeter commands to outputs
        final_outputs.extend(cmd_list_delimeter)
        # adds pulse width commands to outputs
        final_outputs.extend(cmd_list_pulse)
        final_outputs.extend(cmd_list_off)
        chunk_number += 4

    return final_outputs,chunk_number

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

bool_list,num_chunks = createCommands(int(steps),int(pulse_width))
