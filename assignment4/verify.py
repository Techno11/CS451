arr = []

with open('addresses.txt') as f:
    lines = f.readlines()
    for line in lines:
        arr.append(int(line))

frames = []

def calc_pagefault(i):
    m = i // 16

	# simple LRU
    if m in frames:
        frames.remove(m)
        frames.append(m)
        
        return False
    else:
        if(len(frames) > 8):
            frames.pop(0) # remove oldest

        print("Page fault: " + str(m))

        frames.append(m)
        return True



faults = 0

for i in arr:
    fault = calc_pagefault(i)

    if fault:
        faults += 1

print("Total faults: " + str(faults) + " out of " + str(len(arr)) + " references.")
print("Fault rate: " + str(faults / len(arr) * 100) + "%")
	