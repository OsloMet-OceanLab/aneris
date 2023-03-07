
with open('test.wav', 'rb') as file:
    read = file.read()[:44]


with open('test.wav', 'rb') as file:
    import numpy as np
    print(np.frombuffer(file.read()[44:], dtype=np.int16))
    #print(file.read()[4444:4488].hex())

print(f"Bytes 1-4: {read[:4]}")
print(f"Bytes 5-8: {int.from_bytes(read[4:8], 'little')}") # this is variable
print(f"Bytes 9-12: {read[8:12]}")
print(f"Bytes 13-16: {read[12:16]}")
print(f"Bytes 17-20: {int.from_bytes(read[16:20], 'little')}")
print(f"Bytes 21-22: {int.from_bytes(read[20:22], 'little')}")
print(f"Bytes 23-24: {int.from_bytes(read[22:24], 'little')}")
print(f"Bytes 25-28: {int.from_bytes(read[24:28], 'little')}")
print(f"Bytes 29-32: {int.from_bytes(read[28:32], 'little')}")
print(f"Bytes 33-34: {int.from_bytes(read[32:34], 'little')}")
print(f"Bytes 35-36: {int.from_bytes(read[34:36], 'little')}")
print(f"Bytes 37-40: {read[36:40]}")
print(f"Bytes 41-44: {int.from_bytes(read[40:44], 'little')}") # so is this
