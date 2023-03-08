# -*- coding: utf-8 -*-
"""
Created on Wed Mar  8 13:26:40 2023

@author: salve
"""

from logging import warning
from http.server import BaseHTTPRequestHandler
from urllib.parse import urlsplit
from socket import socket, AF_UNIX, SOCK_DGRAM
from bit_converter import bytes_to_16
#from datetime import datetime

HOME_DIR =	'/home/pi/Desktop/aneris-bachelorprosjekt/service/'
WEB_DIR =	HOME_DIR + 'web/'
DOCS_DIR =	WEB_DIR + 'docs/'
LOG_FILE =	HOME_DIR + 'aneris.log'
SOCKET =	'/var/run/aneris.sock'

class StreamingHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        path, query = urlsplit(self.path).path, urlsplit(self.path).query
        try:
            get_params = dict(query.split('=') for query in query.split('&'))
            if 'content' not in get_params:
                get_params['content'] = 'video'
            #if 'timestamp' not in get_params:
                #get_params['timestamp'] = 'false'
        except ValueError:
            get_params = {'content': 'video'}

        if path == '/' or path == '/index.html' or path == '/index':
            try:
                with open(WEB_DIR + 'index.html', 'rb') as index:
                    self.send_response(200)
                    self.send_header('Content-Type', 'text/html')
                    self.end_headers()
                    self.wfile.write(index.read())
            except FileNotFoundError:
                self.send_response(404)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write('Error: index.html does not exist'.encode())
            
        elif path == '/stream':
            if get_params['content'] == 'video':
                self.serve_video()

            elif get_params['content'] == 'audio':
                self.stream_audio()

        elif path == '/video':
            self.serve_video()

        elif path == '/audio':
            self.serve_audio()

        elif path == '/logs':
            try:
                with open(LOG_FILE, 'rb') as log:
                    self.send_response(200)
                    self.send_header('Content-Type', 'text/plain')
                    self.end_headers()
                    self.wfile.write(log.read())
            except FileNotFoundError:
                self.send_response(404)
                self.send_header('Content-type', 'text/plain')
                self.end_headers()
                self.wfile.write('Error: log file does not exist'.encode())

        elif path == '/console':
            self.serve_console('GET');
            
        elif path == '/docs' or path == '/docs/index.html':
            self.serve_docs(path, query)
            
        else:
            self.send_error(404)
            self.end_headers()

    def do_POST(self):
        if self.path == '/console':
            content_length = int(self.headers['Content-Length'])
            post = dict(x.split(b'=') for x in self.rfile.read(content_length).split(b';'))
            self.serve_console('POST', post)
            self.wfile.write('This is POST request. '.encode())
            self.wfile.write('Received: '.encode())
            for x in post:
                self.wfile.write(f'{x}: {post[x]}'.encode())
            if b'command' in post:
                with socket(AF_UNIX, SOCK_DGRAM) as sock:
                    sock.connect(SOCKET)
                    sock.sendall(post[b'command'])
            else:
                self.wfile.write('Invalid post sent'.encode())

        else:
            self.send_error(404)
            self.end_headers()

    # Get params for stream:
    # content: video, audio, videoaudio, default: videoaudio
    # timestamp: true, false (ignored when content: audio), default: true
    # N.B. timestamp has been temporarily disabled

    def serve_video(self):
        global videoOutput
        self.send_response(200)
        self.send_header('Age', 0)
        self.send_header('Cache-Control', 'no-cache, private')
        self.send_header('Pragma', 'no-cache')
        self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
        self.end_headers()
        try:
            while True:
                # lets us annotate time on top of the frame, but
                # latest choice is forced on all users
                #if get_params['timestamp'] == 'true':
                    #camera.annotate_text = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                #else:
                    #camera.annotate_text = ""
                with videoOutput.condition:
                    videoOutput.condition.wait()
                    frame = videoOutput.frame
                self.wfile.write(b'--FRAME\r\n')
                self.send_header('Content-Type', 'image/jpeg')
                self.send_header('Content-Length', len(frame))
                self.end_headers()
                self.wfile.write(frame)
                self.wfile.write(b'\r\n')
        except Exception as e:
            warning('Removed streaming client %s: %s', self.client_address, str(e))

    def serve_audio(self): # might not work, needs testing
        global audioOutput
        self.send_response(200)
        self.send_header('Age', 0)
        self.send_header('Cache-Control', 'no-cache, private')
        self.send_header('Pragma', 'no-cache')
        self.send_header('Content-Type', 'audio/x-wav')
        self.end_headers()
        try:
            while True:
                with audioOutput.condition:
                    audioOutput.condition.wait()
                    frame = audioOutput.frame
                self.send_header('Content-Type', 'audio/x-wav')
                self.end_headers()
                self.wfile.write(frame)
        except Exception as e:
            warning('Removed streaming client %s: %s', self.client_address, str(e))

    def serve_docs(self, docs, query):
        try:
            with open(DOCS_DIR + 'index.html', 'rb') as index:
                self.send_response(200)
                self.send_header('Content-Type', 'text/html')
                self.end_headers()
                self.wfile.write(index.read())
        except FileNotFoundError:
            self.send_response(404)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write('Error: index.html does not exist'.encode())

    def serve_console(self, method, post = None):
        if method == 'POST':
            pass
        try:
            with open(WEB_DIR + 'console.html', 'rb') as index:
                self.send_response(200)
                self.send_header('Content-Type', 'text/html')
                self.end_headers()
                self.wfile.write(index.read())
        except FileNotFoundError:
            self.send_response(404)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write('Error: console.html does not exist'.encode())
