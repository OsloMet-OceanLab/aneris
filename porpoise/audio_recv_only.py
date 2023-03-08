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
            i += 1
            message, _ = sock.recvfrom(1536)
            data = message[5:]

            scnt = int(data.hex()[22:26], 16)
            raw = data[13:]
            
            d += raw
            raw_size += scnt * 3
            
            if i % 1000 == 0:
                with open('test', 'wb') as file:
                    file.write(d)
                    break

    except KeyboardInterrupt:
        print("Done")
            
