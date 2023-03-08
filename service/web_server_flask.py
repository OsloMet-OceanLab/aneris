# -*- coding: utf-8 -*-
"""
Created on Wed Mar  8 12:45:23 2023

@author: salve
"""

from flask import Flask, Response, render_template
from VideoStreamOutput import VideoStreamOutput
from picamera import PiCamera
from socket import socket, AF_INET, AF_UNIX, SOCK_DGRAM
from bit_converter import bytes_to_16

HOME_DIR =	'/home/pi/Desktop/aneris-bachelorprosjekt/service/'
WEB_DIR =	HOME_DIR + 'web/'
DOCS_DIR =	WEB_DIR + 'docs/'
LOG_FILE =	HOME_DIR + 'aneris.log'
SOCKET =	'/var/run/aneris.sock'
HOST, PORT = '', 5453

app = Flask(__name__)

@app.route('/index')
def index():
    return render_template('web/index.html')

@app.route('/video')
def video():
    def gen():
        videoOutput = VideoStreamOutput()
        with PiCamera(resolution='1280x720', framerate=30) as camera:
            camera.start_recording(videoOutput, format='mjpeg') # this type of format uses lossy compression so it might or might not be suited to opencv image analysis
            while True:
                with videoOutput.condition:
                    videoOutput.condition.wait()
                    frame = videoOutput.frame
                yield(b'--FRAME\r\n'
                      b'Content-Type', 'image/jpeg\r\n\r\n' + 
                      frame + b'\r\n')
            camera.stop_recording()

    return Response(gen(), mimetype = 'multipart/x-mixed-replace; boundary=FRAME')

@app.route('/audio')
def audio():
    def sound():
        with socket(AF_INET, SOCK_DGRAM) as sock:
            while True:
                sock.bind((HOST, PORT))
                message, _ = sock.recvfrom(1536)
                scnt = scnt = int(message.hex()[32:36], 16)
                data = bytes_to_16(message[18:], scnt*3, True)
                yield(data)
    
    return Response(sound(), mimetype = 'audio/x-wav')

@app.route('/console')
def console():
    pass

@app.route('/logs')
def logs():
    pass

@app.route('/docs')
def docs():
    pass

if __name__ == "__main__":
    app.run(host = '0.0.0.0', debug = True, threaded = True, port = 5000)