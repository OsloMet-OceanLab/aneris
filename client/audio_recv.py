from socket import socket, AF_INET, SOCK_DGRAM
import sys, wave

HOST, PORT = '', 5453

d = ''
i = 0

with socket(AF_INET, SOCK_DGRAM) as sock:
    sock.bind((HOST, PORT))
    print("Bound")
    try:
        while True:
            i += 1
            print(i)
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
            
            d += raw.hex()
            
            if i % 2500 == 0:
                header = '52494646 totsize 57415645666d742010000000010001000077010000ee02000200100064617461 filsize '

                length = len(d)
                header = header.replace(' totsize ', hex(length+44*2-8*2)[2:])
                print(hex(length+44*2-8*2)[2:])
                header = header.replace(' filsize ', hex(length)[2:])
                print(hex(length)[2:])
                print(header, len(header)/2)

                wavfile = header + d
                
                break
        
        with open('test.wav', 'wb') as stream:
            stream.write(bytes.fromhex(wavfile))

    except KeyboardInterrupt:
        print("Done")
            
