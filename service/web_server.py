# original version from https://picamera.readthedocs.io/en/latest/recipes2.html#web-streaming
# make sure the legacy camera stack is enabled if it doesn't work
# include copyright notice here

from io import BytesIO
from picamera import PiCamera
from logging import warning
from socketserver import ThreadingMixIn
from threading import Condition
from http.server import BaseHTTPRequestHandler, HTTPServer
from sys import exit, argv

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
		if self.path == '/':
			self.show_index()
			
		elif self.path == '/index.html':
			self.show_index()
			
		elif self.path == '/stream':
			self.send_response(200)
			self.send_header('Age', 0)
			self.send_header('Cache-Control', 'no-cache, private')
			self.send_header('Pragma', 'no-cache')
			self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
			self.end_headers()
			try:
				global output
				while True:
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
					
		elif self.path == '/log':
			try:
				with open('/home/pi/Desktop/log.txt', 'rb') as log:
					self.send_response(200)
					self.send_header('Content-Type', 'text/plain')
					self.end_headers()
					self.wfile.write(log.read())
			except FileNotFoundError as e:
				self.send_response(404)
				self.send_header('Content-type', 'text/plain')
				self.end_headers()
				self.wfile.write('Error: log file does not exist'.encode())
				
		elif self.path == '/metadata':
			self.send_error(418)
			self.end_headers()
			
		elif self.path == '/console':
			self.send_error(418)
			self.end_headers()
			
		elif self.path == '/docs':
			self.send_error(418)
			self.end_headers()
			
		else:
			self.send_error(418)
			self.end_headers()
			
	def show_index(self):
		try:
			with open('index.html', 'rb') as index:
				self.send_response(200)
				self.send_header('Content-Type', 'text/html')
				self.end_headers()
				self.wfile.write(index.read())
					
		except FileNotFoundError as e:
			self.send_response(404)
			self.send_header('Content-type', 'text/plain')
			self.end_headers()
			self.wfile.write('Error: index page does not exist'.encode())

class StreamingServer(ThreadingMixIn, HTTPServer):
	allow_reuse_address = True
	daemon_threads = True

output = StreamingOutput()

def main():
	if len(argv) != 2 or not argv[1].isdigit():
		print(f"Usage: python3 {argv[0]} <port>")
		exit(1)
	with PiCamera(resolution='1280x720', framerate=30) as camera:
		global output
		camera.start_recording(output, format='mjpeg') # this type of format uses lossy compression so it might not be suited to opencv image analysis
		try:
			address = ('', int(argv[1])) # ip, port
			server = StreamingServer(address, StreamingHandler)
			print('Stream started successfully')
			server.serve_forever()
		except KeyboardInterrupt:
			print('Received Keyboard Interrupt, quitting...')
		except Exception as e:
			print('Couldn\'t start stream')
			print(f'Additional information: {str(e)}')
			camera.stop_recording()
			exit(2)
		camera.stop_recording()
		exit(0)

if __name__ == "__main__":
	main()
