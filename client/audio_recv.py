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
            
            file = b'RIFF 0413 WAVE fmt  0000 01 01 17700 192000 02 0f data 0000'
            
            d += raw
            
            if i % 1000 == 0:
                break
        
        with wave.open('test.wav', 'wb') as wavfile:
            wavfile.setparams((1, 2, 96_000, 0, 'NONE', 'NONE'))
            wavfile.writeframes(d)
        print(d)

    except KeyboardInterrupt:
        print("Done")
            
