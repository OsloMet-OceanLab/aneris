from http.server import BaseHTTPRequestHandler, HTTPServer
from sys import exit, argv

class Handler(BaseHTTPRequestHandler):
	def do_GET(self):
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
			self.wfile.write("Error: log file does not exist".encode())
				
def main():
	if len(argv) != 2 or not argv[1].isdigit():
		print(f"Usage: python3 {argv[0]} <port>")
		exit(1)
	address = ('', int(argv[1]))
	with HTTPServer(address, Handler) as server:
		while True:
			try:
				server.serve_forever()
			except KeyboardInterrupt:
				print('Received Keyboard Interrupt, quitting...')
				exit(0)
			except Exception as e:
				print('Couldn\'t start stream')
				print('Additional information: %s', str(e))
				exit(2)

if __name__ == "__main__":
	main()
