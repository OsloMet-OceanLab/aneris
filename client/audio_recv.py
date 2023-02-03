import socket
import pyaudio
IP_ADDR = "10.44.6.93"
PI = ''
PORT = 5000

p = pyaudio.PyAudio()
CHUCK = 1024*4
FORMAT = pyaudio.paInt24
CHANNELS = 2
RATE = 288000
stream = p.open(format=FORMAT, channels=CHANNELS, rate=RATE, output=True, frames_per_buffer=CHUNK)

with socket.socket() as sock:
	sock.connect((IP_ADDR, PORT))
