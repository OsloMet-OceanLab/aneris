# original version from https://picamera.readthedocs.io/en/latest/recipes2.html#web-streaming
# make sure the legacy camera stack is enabled if it doesn't work
# include copyright notice here


from io import BytesIO
from picamera import PiCamera
from logging import warning
from socketserver import ThreadingMixIn
from threading import Condition
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlsplit
from socket import socket, AF_UNIX, SOCK_DGRAM
#from datetime import datetime

HOME_DIR = '/home/pi/Desktop/aneris-bachelorprosjekt/service/'
WEB_DIR =	HOME_DIR + 'web/'
DOCS_DIR =	WEB_DIR + 'docs/'
LOG_FILE =	HOME_DIR + 'aneris.log'
SOCKET = '/var/run/aneris.sock'

class StreamingOutput:
	def __init__(self):
		self.frame = None
		self.buffer = BytesIO()
		self.condition = Condition()

	def write(self, buf):
		if buf.startswith(b'\xff\xd8'):
			self.buffer.truncate()
			with self.condition:
				self.frame = self.buffer.getvalue()
				self.condition.notify_all()
			self.buffer.seek(0)
		return self.buffer.write(buf)

class StreamingHandler(BaseHTTPRequestHandler):
	def do_GET(self):
		path, query = urlsplit(self.path).path, urlsplit(self.path).query
		try:
			get_params = dict(query.split('=') for query in query.split('&'))
			if 'content' not in get_params:
				get_params['content'] = 'video'
			if 'timestamp' not in get_params:
				get_params['timestamp'] = 'false'
		except ValueError:
			get_params = {'content': 'video',
							'timestamp': 'false'}

		if path == '/' or path == '/index.html' or path == '/index':
			self.serve_index()
			
		# Get params for stream:
		# content: video, audio, videoaudio, default: videoaudio
		# timestamp: true, false (ignored when content: audio), default: true
		# N.B. timestamp has been temporarily disabled
			
		elif path == '/stream':
			if get_params['content'] == 'video':
				global output
				self.send_response(200)
				self.send_header('Age', 0)
				self.send_header('Cache-Control', 'no-cache, private')
				self.send_header('Pragma', 'no-cache')
				self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
				self.end_headers()
				try:
					while True:
						# lets us annotate time on top of the frame, but
						# this choice is forced on all users
						#if get_params['timestamp'] == 'true':
							#camera.annotate_text = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
						#else:
							#camera.annotate_text = ""
						with output.condition:
							output.condition.wait()
							frame = output.frame
						self.wfile.write(b'--FRAME\r\n')
						self.send_header('Content-Type', 'image/jpeg')
						self.send_header('Content-Length', len(frame))
						self.end_headers()
						self.wfile.write(frame)
						self.wfile.write(b'\r\n')
				except Exception as e:
					warning(
						'Removed streaming client %s: %s',
						self.client_address, str(e))
							
			elif get_params['content'] == 'videoaudio':
				self.send_error(418)
				self.end_headers()
				
			elif get_params['content'] == 'audio':
				self.send_error(418)
				self.end_headers()

		elif path == '/logs':
			try:
				with open(LOG_FILE, 'rb') as log:
					self.send_response(200)
					self.send_header('Content-Type', 'text/plain')
					self.end_headers()
					self.wfile.write(log.read())
			except FileNotFoundError as e:
				self.send_response(404)
				self.send_header('Content-type', 'text/plain')
				self.end_headers()
				self.wfile.write('Error: log file does not exist'.encode())
				
		elif path == '/metadata':
			self.send_error(418)
			self.end_headers()
			
		elif path == '/console':
			self.serve_console();
			
		elif path == '/docs' or path == '/docs/index.html':
			self.serve_docs(path)
			
		else:
			self.send_error(404)
			self.end_headers()

	def do_POST():
		if self.path == '/console':
			content_length = int(self.headers['Content-Length'])
            post = dict(x.split(b'=') for x in self.rfile.read(content_length).split(b';'))
            self.send_response(200)
            self.end_headers()
			serve_console()
			if 'command' not in post:
				pass
            self.wfile.write('This is POST request. '.encode())
            self.wfile.write('Received: '.encode())
            for x in post:
                self.wfile.write(f"{x}: {post[x]}".encode())
			with socket(AF_UNIX, SOCK_DGRAM) as sock:
				sock.connect(SOCKET)
				sock.sendall(post['command'])

		else:
			self.send_error(404)
			self.end_headers()
			
	def serve_index(self):
		try:
			with open(WEB_DIR + 'index.html', 'rb') as index:
				self.send_response(200)
				self.send_header('Content-Type', 'text/html')
				self.end_headers()
				self.wfile.write(index.read())
		except FileNotFoundError as e:
			self.send_response(404)
			self.send_header('Content-type', 'text/plain')
			self.end_headers()
			self.wfile.write('Error: index.html does not exist'.encode())
			
	def serve_docs(self, doc):
		try:
			with open(DOCS_DIR + 'index.html', 'rb') as index:
				self.send_response(200)
				self.send_header('Content-Type', 'text/html')
				self.end_headers()
				self.wfile.write(index.read())
		except FileNotFoundError as e:
			self.send_response(404)
			self.send_header('Content-type', 'text/plain')
			self.end_headers()
			self.wfile.write('Error: index.html does not exist'.encode())

	def serve_console(self):
		try:
			with open(WEB_DIR + 'console.html', 'rb') as index:
				self.send_response(200)
				self.send_header('Content-Type', 'text/html')
				self.end_headers()
				self.wfile.write(index.read())
		except FileNotFoundError as e:
			self.send_response(404)
			self.send_header('Content-type', 'text/plain')
			self.end_headers()
			self.wfile.write('Error: console.html does not exist'.encode())


class StreamingServer(ThreadingMixIn, HTTPServer):
	allow_reuse_address = True
	daemon_threads = True

output = StreamingOutput()

def serve(port = 0):
	if port <= 0 and not port.isdigit():
		return 1
	
	global output
	with PiCamera(resolution='1280x720', framerate=30) as camera:
		camera.start_recording(output, format='mjpeg') # this type of format uses lossy compression so it might or might not be suited to opencv image analysis
		try:
			address = ('', int(port)) # ip, port
			server = StreamingServer(address, StreamingHandler)
			print('Stream started successfully')
			server.serve_forever()
		except Exception as e:
			print('Couldn\'t start stream')
			print(f'Additional information: {str(e)}')
			camera.stop_recording()
			return 2
		camera.stop_recording()
		return 0
