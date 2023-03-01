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
            data = message[5:]

            scnt = int(data.hex()[22:26], 16)
            raw = data[13:]
            
            d += raw
            raw_size += scnt * 3
            
            import matplotlib.pyplot as plt, numpy as np
            
            d3 = b''
            
            for i in range(0, scnt, 3):
                tmpint = _24to32(d[i:i+3])
                d3 += tmpint.to_bytes(4, 'little', signed=True)

            d2 = np.frombuffer(d[:512], dtype=np.int32)
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
            
