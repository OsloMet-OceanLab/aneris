# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

from socket import socket, AF_INET, SOCK_DGRAM
from pyaudio import PyAudio, paInt24, paInt16, paInt32
import winsound
from os import name
"""
from ao import AudioDevice
def play_data(filename, first_sec, second_sec):
  import ao
  from ao import AudioDevice 
  dev = AudioDevice(2, bits=16, rate=16000,channels=1)
  f = open(filename, 'r')
  data_len = (second_sec-first_sec)*32000
  f.seek(32000*first_sec)
  data = f.read(data_len)
  dev.play(data)
  f.close()
"""
p = PyAudio()
FORMAT = paInt16
CHANNELS = 1
RATE = 96_000
CHUNK = 999
stream = p.open(format = FORMAT, channels = CHANNELS, rate = RATE, output = True, frames_per_buffer = CHUNK)

HOST, PORT = '', 5453

with socket(AF_INET, SOCK_DGRAM) as sock:
    sock.bind((HOST, PORT))
    while True:
        try:
            message, address = sock.recvfrom(4096)
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
            raw = data[13:(3*scnt*chmap)+13]
            if name == 'nt':
                winsound.PlaySound(raw, winsound.SND_NOSTOP)
            elif name == 'posix':
                #stream.write(raw)
                pass
                
        except KeyboardInterrupt:
            print("Done")
            break
		
stream.close()
p.terminate()
