# -*- coding: utf-8 -*-
"""
Created on Wed Feb 22 10:11:51 2023

@author: OceanLab
"""

from socket import socket, AF_INET, SOCK_DGRAM
import sys

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
            message, address = sock.recvfrom(1536)
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
            

            d += raw
            raw_size += scnt * 3
            i += 1
            
            if i % 100 == 0:
                print(i)
            
            if False:#i % 6 == 0:
                import struct
                d = struct.pack(f'<{raw_size//2}h', *struct.unpack(f'>{raw_size//2}h', d)) # it's possible i'm encoding this big endian audio as little endian
                print(d[:8].hex())
                print(raw[:8].hex())
                sys.exit(0)
            
            if i % 1000 == 0:
                print(raw_size)
                header = genHeader(96_000, 16, 1, raw_size)
                import struct
                d = struct.pack(f'<{raw_size//2}h', *struct.unpack(f'>{raw_size//2}h', d)) # it's possible i'm encoding this big endian audio as little endian
                wavfile = header + d
                
                break

        with open('test3.wav', 'wb') as stream:
            stream.write(wavfile)
      
    except KeyboardInterrupt:
        print("Done")
            
