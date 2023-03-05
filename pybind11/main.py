import modul

print(modul.add(1, 2))

import bit_converter, socket

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
