from socket import socket, AF_INET, SOCK_DGRAM
import sys, wave

HOST, PORT = '', 5453

d = b''
i = 0

with socket(AF_INET, SOCK_DGRAM) as sock:
    sock.bind((HOST, PORT))
    try:
        while True:
            i += 1
            message, address = sock.recvfrom(4096)
            sync = message.hex()[0:4]
            #print(f"Sync: {sync}")
            size = int(message.hex()[4:8], 16)
            #print(f"Size: {size}")
            if message.hex()[8:10] == '00':
                dtype = 'Unknown'
            elif message.hex()[8:10] == '01':
                dtype = 'DAQ'
            elif message.hex()[8:10] == '02':
                dtype = 'Noise'
            #print(f"Data type: {dtype}")

            data = message[5:size]

            if data.hex()[0:2] == '00':
                dformat = 'Unknown'
            elif data.hex()[0:2] == '01':
                dformat = 'PCM16'
            elif data.hex()[0:2] == '02':
                dformat = 'PCM24'
            #print(f"Data format: {dformat}")
            seq = data.hex()[2:6]
            #print(f"Sequence number: {seq}")
            sr = int(data.hex()[6:14], 16)
            #print(f"Sample rate: {sr}")
            chmap = int(data.hex()[14:22], 16)
            #print(f"Enabled channels: {chmap}")
            scnt = int(data.hex()[22:26], 16)
            #print(f"Samples per channel: {scnt}")
            raw = data[13:(3*scnt*chmap)+13]
            #print(raw.hex())
            
            header = '52494646 24f00000 57415645 666d7420 10000000 0100 0100 00770100 00ee0200 0200 1000 64617461 00f00000'
                     '52494646 240f1e00 57415645 666d7420 10000000 0100 0100 00770100 00ee0200 0200 1000 64617461 000f1e00'
            
            d += raw
            
            if i % 1000 == 0:
                header += d
                break
        
        with wave.open('test.wav', 'wb') as wavfile:
            wavfile.setparams((1, 2, 96_000, 0, 'NONE', 'NONE'))
            wavfile.writeframes(d)
        print(d)

    except KeyboardInterrupt:
        print("Done")
            
