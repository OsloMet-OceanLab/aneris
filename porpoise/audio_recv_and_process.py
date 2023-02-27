# -*- coding: utf-8 -*-
"""
Created on Mon Feb 27 10:58:16 2023

@author: salve
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

d = bytearray()
i = 0
raw_size = 0

with socket(AF_INET, SOCK_DGRAM) as sock:
    sock.bind((HOST, PORT))
    print("Bound")
    try:
        while True:
            message, address = sock.recvfrom(1536)
            sync = message.hex()[0:4]
            print(f"Sync: {sync}")
            size = int.from_bytes(message[2:4], 'big')
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
            sr = int.from_bytes(message[3:7], 'big')
            print(f"Sample rate: {sr}")
            chmap = int.from_bytes(message[7:11], 'big')
            print(f"Enabled channels: {chmap}")
            scnt = int.from_bytes(message[11:13], 'big')
            print(f"Samples per channel: {scnt}")
            raw = data[13:(3*scnt*chmap)+13]
            print(raw.hex())
            sys.exit()
            
            d += raw
            raw_size += scnt * 3
            i += 1
            
            if i % 100 == 0:
                print(i)

            if i % 1000 == 0:
                print(raw_size)
                header = genHeader(96_000, 16, 1, raw_size)
                wavfile = header + d
                
                break
            
        with open('test2.wav', 'wb') as stream:
            stream.write(wavfile)

    except KeyboardInterrupt:
        print("Done")
            
