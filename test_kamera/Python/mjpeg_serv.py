# original version from https://picamera.readthedocs.io/en/latest/recipes2.html#web-streaming
# make sure the legacy camera stack is enabled if it doesn't work
# include copyright notice here

from io import BytesIO
from picamera import PiCamera
from logging import warning
from socketserver import ThreadingMixIn
from threading import Condition
from http.server import BaseHTTPRequestHandler, HTTPServer

ADDRESS, PORT = '', 5000

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
		if self.path == '/stream':
			self.send_response(200)
			self.send_header('Age', 0)
			self.send_header('Cache-Control', 'no-cache, private')
			self.send_header('Pragma', 'no-cache')
			self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
			self.end_headers()
			try:
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
		else:
			self.send_error(404)
			self.end_headers()

class StreamingServer(ThreadingMixIn, HTTPServer):
	allow_reuse_address = True
	daemon_threads = True

output = StreamingOutput()

def main():
	with PiCamera(resolution='640x480', framerate=24) as camera:
		camera.start_recording(output, format='mjpeg') # this type of format uses lossy compression so it might not be suited to opencv image analysis
		try:
			address = (ADDRESS, PORT)
			server = StreamingServer(address, StreamingHandler)
			print('Stream started successfully')
			server.serve_forever()
		except KeyboardInterrupt:
			print('Received Keyboard Interrupt, quitting...')
		except Exception as e:
			print('Couldn\'t start stream')
			print('Additional information: %s', str(e))
		finally:
			camera.stop_recording()
			
if __name__ == "__main__":
	main()
