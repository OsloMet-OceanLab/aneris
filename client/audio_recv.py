# -*- coding: utf-8 -*-
"""
Created on Wed Feb 22 10:11:51 2023

@author: salve
"""

from socket import socket, AF_INET, SOCK_DGRAM
import sys
from bit_converter import bytes_to_16

def genHeader(sampleRate, bitsPerSample, channels, samples):
    datasize = samples #* channels * bitsPerSample // 8
    endian = 'little'
    sign = True
    o = bytes("RIFF", 'ascii')
    o += (datasize + 36).to_bytes(4, endian, signed = sign)
    o += bytes("WAVE", 'ascii')
    o += bytes("fmt ", 'ascii')
    o += (16).to_bytes(4, endian, signed = sign)
    o += (1).to_bytes(2, endian, signed = sign)
    o += (channels).to_bytes(2, endian, signed = sign)
    o += (sampleRate).to_bytes(4, endian, signed = sign)
    o += (sampleRate * channels * bitsPerSample // 8).to_bytes(4, endian, signed = sign)
    o += (channels * bitsPerSample // 8).to_bytes(2, endian, signed = sign)
    o += (bitsPerSample).to_bytes(2, endian, signed = sign)
    o += bytes("data", 'ascii')
    o += (datasize).to_bytes(4, endian, signed = sign)
    return o

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
            raw = data[13:]#(3*scnt*chmap)+13]
            print(raw.hex())
            
            d += bytes_to_16(raw)
            raw_size += scnt * 3
            i += 1
            
            if i % 600 == 0:
                import matplotlib.pyplot as plt, numpy as np
                
                plt.figure()
                d2 = np.frombuffer(d, dtype=np.int32)
                plt.plot(d2)
                print(d2)
                print(max(d2))
                print(min(d2))
                print(min(abs(d2)))
                plt.xlabel('Sample index')
                plt.ylabel('Amplitude')
                plt.title('Waveform')
                plt.show()
                sys.exit(0)

    except KeyboardInterrupt:
        print("Done")
            
