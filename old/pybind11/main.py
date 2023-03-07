import bit_converter as bc

"""
with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:
    sock.bind(('', 5453))
    print("Bound")
    try:
        while True:
            message, _ = sock.recvfrom(1536)
            data = message[5:]

            scnt = int(data.hex()[22:26], 16)
            raw = data[13:]
            
            raw_16 = bit_converter.numto16(buf = raw, size = scnt * 3, invertEndianness = True)
"""

b = b'\xff\xfd\xf8\xff\xfe\x58'


print(bc.numto16(b, len(b), True).hex())

print(bc.return_bytes(b, len(b)).hex())
