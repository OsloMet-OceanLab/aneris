# -*- coding: utf-8 -*-
"""
Created on Tue Feb 28 13:43:59 2023

@author: salve
"""

from socket import socket, AF_INET, SOCK_DGRAM

HOST, PORT = '', 5453

d = b''
i = 0
raw_size = 0

with socket(AF_INET, SOCK_DGRAM) as sock:
    sock.bind((HOST, PORT))
    print("Bound")
    try:
        while True:
            message, _ = sock.recvfrom(1536)
            size = int(message.hex()[4:8], 16)

            data = message[5:size]

            sr = int(data.hex()[6:14], 16)
            chmap = int(data.hex()[14:22], 16)
            scnt = int(data.hex()[22:26], 16)
            raw = data[13:]#(3*scnt*chmap)+13]
            #print(raw.hex())
            
            d += raw
            raw_size += scnt * 3
            i += 1
            
            if i % 6 == 0:
                
                import matplotlib.pyplot as plt, numpy as np, audioop

                d2 = np.frombuffer(d, dtype=np.int32)
                print(d2)
                
                plt.figure()
                plt.plot(d2)
                plt.xlabel('Sample index')
                plt.ylabel('Amplitude')
                plt.title('Waveform')
                plt.show()
                
                break

    except KeyboardInterrupt:
        print("Done")
            
