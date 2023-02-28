# -*- coding: utf-8 -*-
"""
Created on Tue Feb 28 13:43:59 2023

@author: salve
"""

from socket import socket, AF_INET, SOCK_DGRAM

HOST, PORT = '', 5453

d = b''
#i = 0
raw_size = 0

with socket(AF_INET, SOCK_DGRAM) as sock:
    sock.bind((HOST, PORT))
    print("Bound")
    try:
        while True:
            message, _ = sock.recvfrom(1536)
            """
            with open('packet', 'wb') as file:
                file.write(message)
                break
            "
            sync = message.hex()[0:4]
            print(f"Sync: {sync}")
            size = int(message.hex()[4:8], 16)
            print(f"Size: {size}")
            if message.hex()[8:10] == '00':
                dtype = 'Unknown'
            elif message.hex()[8:10] == '01':
                dtype = 'DAQ'
            elif message.hex()[8:10] == '02':
                dtype = 'Noise'
            print(f"Data type: {dtype}")

            data = message[5:size]

            if data.hex()[0:2] == '00':
                dformat = 'Unknown'
            elif data.hex()[0:2] == '01':
                dformat = 'PCM16'
            elif data.hex()[0:2] == '02':
                dformat = 'PCM24'
            print(f"Data format: {dformat}")
            seq = data.hex()[2:6]
            print(f"Sequence number: {seq}")
            sr = int(data.hex()[6:14], 16)
            print(f"Sample rate: {sr}")
            chmap = int(data.hex()[14:22], 16)
            print(f"Enabled channels: {chmap}")
            scnt = int(data.hex()[22:26], 16)
            print(f"Samples per channel: {scnt}")
            
            print(message[:18].hex())
            
            break

            """
            
            data = message[5:]

            scnt = int(data.hex()[22:26], 16)
            raw = data[13:]
            
            d += raw#[:scnt * 3 - 1]
            raw_size += scnt * 3
            #i += 1

            #if i % 6 == 0:

            import matplotlib.pyplot as plt, numpy as np

            import struct
            #d = struct.pack(f'<{raw_size//2}h', *struct.unpack(f'>{raw_size//2}h', d)) # it's possible i'm encoding this big endian audio as little endian

            d2 = np.frombuffer(d[:512], dtype=np.int32)
            #d2 = d3[np.abs(d3) < 20_000_000]
            x = np.arange(1, d2.size + 1)
            print(d2)
            
            plt.figure()
            plt.plot(x, d2)
            plt.xlabel('Sample index')
            plt.ylabel('Amplitude')
            plt.title('Waveform')
            plt.show()
            
            break

    except KeyboardInterrupt:
        print("Done")
            
