# -*- coding: utf-8 -*-
"""
Created on Fri Feb 24 12:16:15 2023

@author: salve
"""

from socket import socket, AF_INET, SOCK_DGRAM
from socketserver import ThreadingMixIn
from http.server import BaseHTTPRequestHandler, HTTPServer

def parse(sock):
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
    
    return raw, scnt

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

class StreamingHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        self.serve_audio()
        
    def serve_audio(self):
        self.send_response(200)
        self.send_header('Age', 0)
        self.send_header('Cache-Control', 'no-cache, private')
        self.send_header('Pragma', 'no-cache')
        self.send_header('Content-Type', 'audio/x-wav')
        self.end_headers()
        
        HOST, PORT = '', 5453
        d = b''
        i = 0
        raw_size = 0
        with socket(AF_INET, SOCK_DGRAM) as sock:
            sock.bind((HOST, PORT))
            print("Bound")
            try:
                while True:
                    raw, scnt = parse(sock)
                    d += raw
                    raw_size += scnt * 3
                    print(len(d.hex())/2)
                    i += 1
                    
                    if i % 100 == 0:
                        print(f'i: {i}')
                    
                    if i % 6 == 0:
                        print(raw_size)
                        header = genHeader(96_000, 16, 1, raw_size)
                        wavfile = header + d
                        
                        print(wavfile)
                        
                        break

            except KeyboardInterrupt:
                print("Done")

class StreamingServer(ThreadingMixIn, HTTPServer):
    allow_reuse_address = True
    daemon_threads = True

def serve(port = 0):
    if port <= 0 and not port.isdigit():
        return 1
    
    try:
        address = ('', int(port)) # ip, port
        server = StreamingServer(address, StreamingHandler)
        print('Stream started successfully')
        server.serve_forever()
    except Exception as e:
        print('Couldn\'t start stream')
        print(f'Additional information: {str(e)}')
        return 2
    return 0

if __name__ == "__main__":
    serve(5000)
