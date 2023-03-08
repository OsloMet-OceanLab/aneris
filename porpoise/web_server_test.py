# -*- coding: utf-8 -*-
"""
Created on Fri Feb 24 12:16:15 2023

@author: salve
"""

from socket import socket, AF_INET, SOCK_DGRAM
from socketserver import ThreadingMixIn
from http.server import BaseHTTPRequestHandler, HTTPServer
from io import BytesIO
from threading import Condition

class AudioStreamOutput:
    def __init__(self):
        self.frame = None
        self.buffer = BytesIO()
        self.condition = Condition()

    def write(self, buf):
        if buf.startswith(b'\x0a\x06'):
            self.buffer.truncate()
            with self.condition:
                self.frame = self.buffer.getvalue()[18:]
                self.condition.notify_all()
            self.buffer.seek(0)
        return self.buffer.write(buf)
    
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

        try:
            with socket(AF_INET, SOCK_DGRAM) as sock:
                sock.bind(('', 80))
                print("Bound")
                self.send_header('Content-Type', 'audio/x-wav')
                self.end_headers()
                self.wfile.write(genHeader(96_000, 16, 1, 102400000))
                try:
                    while True:
                        message, _ = sock.recvfrom(1536)
                        self.wfile.write(message[18:])
                except:
                    pass
        except:
            pass
        
class StreamingServer(ThreadingMixIn, HTTPServer):
    allow_reuse_address = True
    daemon_threads = True

audioOutput = AudioStreamOutput()

def serve(port = 0):
    if port <= 0 and not port.isdigit():
        return 1
    
    try:
        address = ('', int(port)) # ip, port
        server = StreamingServer(address, StreamingHandler)
        print('Stream started successfully')
        server.serve_forever()
    except KeyboardInterrupt as e:
        print('Couldn\'t start stream')
        print(f'Additional information: {str(e)}')
        server.server_close()
        return 2
    server.server_close()
    return 0

if __name__ == "__main__":
    serve(80)
