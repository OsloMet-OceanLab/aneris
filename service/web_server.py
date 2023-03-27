# original version from https://picamera.readthedocs.io/en/latest/recipes2.html#web-streaming
# make sure the legacy camera stack is enabled if it doesn't work
# include copyright notice here

try:
    # camera imports
    from io import BytesIO
    from threading import Condition
    from picamera import PiCamera

    # web server imports
    from logging import warning
    from socketserver import ThreadingMixIn
    from http.server import BaseHTTPRequestHandler, HTTPServer
    from urllib.parse import urlsplit
    from socket import socket, AF_UNIX, SOCK_DGRAM
    #from datetime import datetime
except ImportError as e:
    print('Couldn\'t import all required modules')
    print(f'Additional information: {str(e)}')

__version__ = '1.0.0'

HOME_DIR =	'/etc/aneris/'
WEB_DIR =	HOME_DIR + 'web/'
DOCS_DIR =	WEB_DIR + 'docs/'
LOG_FILE =	'/var/log/aneris/aneris.log'
SOCKET =	'/var/run/aneris.sock'

class VideoStreamBuffer:
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
    
    def write2(self, buf): #h.264 encoding
        if buf.startswith(b'\x00\x00\x00\x01'):
            with self.condition:
                self.buffer.seek(0)
                self.buffer.write(buf)
                self.buffer.truncate()
                self.frame = self.buffer.getvalue()
                self.condition.notify_all()

class StreamingHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        path, query = urlsplit(self.path).path, urlsplit(self.path).query
        try:
            get_params = dict(query.split('=') for query in query.split('&'))
        except ValueError:
            get_params = dict()

        if path == '/' or path == '/index.html' or path == '/index':
            try:
                with open(WEB_DIR + 'index.html', 'rb') as index:
                    self.send_response(200)
                    self.send_header('Content-Type', 'text/html')
                    self.end_headers()
                    self.wfile.write(index.read())
            except FileNotFoundError:
                self.send_error(500,
                                'Error: index file does not exist',
                                f'Check that the index file exists in directory \'{WEB_DIR}\'')
            
        elif path == '/stream':
            self.serve_video()

        elif path == '/logs':
            try:
                with open(LOG_FILE, 'rb') as log:
                    self.send_response(200)
                    self.send_header('Content-Type', 'text/plain')
                    self.end_headers()
                    self.wfile.write(log.read())
            except FileNotFoundError:
                self.send_error(500,
                                'Error: log file does not exist',
                                f'Check that the log file exists in directory \'{LOG_FILE}\'')

        elif path == '/console':
            self.serve_console();
            
        elif path == '/docs' or path == '/docs/index.html':
            self.serve_docs(path, query)
            
        else:
            self.send_error(404)
            self.end_headers()

    def do_POST(self):
        if self.path == '/api':
            try:
                content_length = int(self.headers['Content-Length'])
                post = dict(x.split(b'=') for x in self.rfile.read(content_length).split(b';'))
            except:
                self.send_response(400)
                self.send_header('Content-type', 'application/json')
                self.end_headers()
                response = '{\
                                "error": {\
                                "code": "InvalidPOST",\
                                "message": "The POST request sent to the server did not include a \'command\' parameter"\
                                }\
                            }'
                self.wfile.write(response.encode())
                return

            ### remove after, just for debug
#            self.wfile.write('This is POST request. '.encode())
#            self.wfile.write('Received: '.encode())
#            for x in post:
#                self.wfile.write(f'{x}: {post[x]}'.encode())
            ###

            if b'command' in post:
                try:
                    with socket(AF_UNIX, SOCK_DGRAM) as sock:
                        sock.connect(SOCKET)
                        sock.sendall(post[b'command'])
                    self.send_response(200)
                    self.send_header('Content-type', 'application/json')
                    self.end_headers()
                    response = '{\
                                    "result": {\
                                    "code": "Success",\
                                    "message": "The operation was successful"\
                                    }\
                                }'
                    self.wfile.write(response.encode())
                except:
                    self.send_response(500)
                    self.send_header('Content-type', 'application/json')
                    self.end_headers()
                    response = '{\
                                    "error": {\
                                    "code": "SocketError",\
                                    "message": "The server couldn\'t connect to the socket"\
                                    }\
                                }'
                    self.wfile.write(response.encode())

            else:
                self.send_response(400)
                self.send_header('Content-type', 'application/json')
                self.end_headers()
                response = '{\
                                "error": {\
                                "code": "InvalidPOST",\
                                "message": "The POST request sent to the server did not include a \'command\' parameter"\
                                }\
                            }'
                self.wfile.write(response.encode())

                                

        else:
            self.send_error(404)
            self.end_headers()

    # Get params for stream:
    # content: video, audio, videoaudio, default: videoaudio
    # timestamp: true, false (ignored when content: audio), default: true
    # N.B. timestamp has been temporarily disabled

    def serve_video(self):
        global videoBuffer
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
                with videoBuffer.condition:
                    videoBuffer.condition.wait()
                    frame = videoBuffer.frame
                self.wfile.write(b'--FRAME\r\n')
                self.send_header('Content-Type', 'image/jpeg')
                self.send_header('Content-Length', len(frame))
                self.end_headers()
                self.wfile.write(frame)
                self.wfile.write(b'\r\n')
        except Exception as e:
            warning('Removed streaming client %s: %s', self.client_address, str(e)) # probaably unneeded
    
    def serve_video_2(self):
        global videoBuffer
        self.send_response(200)
        self.send_header('Age', 0)
        self.send_header('Cache-Control', 'no-cache, private')
        self.send_header('Pragma', 'no-cache')
        self.send_header('Content-Type', 'multipart/x-mixed-replace; boundary=FRAME')
        self.end_headers()
        try:
            while True:
                with videoBuffer.condition:
                    videoBuffer.condition.wait()
                    frame = videoBuffer.frame
                self.wfile.write(frame)
        except Exception as e:
            warning('Removed streaming client %s: %s', self.client_address, str(e)) # probably unneeded

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

    def serve_console(self):
        
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

class StreamingServer(ThreadingMixIn, HTTPServer):
    allow_reuse_address = True
    daemon_threads = True

videoBuffer = VideoStreamBuffer()

def serve(port = 0):
    if port <= 0 and not port.isdigit():
        return 1
	
    global videoOutput
    with PiCamera(resolution='1280x720', framerate=30) as camera:
        camera.start_recording(videoBuffer, format='mjpeg') # this type of format uses lossy compression so it might or might not be suited to opencv image analysis
        #camera.start_recording(videoBuffer, format='h264', profile='baseline') # h.264 option
        try:
            return_int = 0
            address = ('', int(port)) # ip, port
            server = StreamingServer(address, StreamingHandler)
            print('Stream started successfully')
            server.serve_forever()
        except Exception as e:
            print('Couldn\'t start stream')
            print(f'Additional information: {str(e)}')
            return_int = 2
        finally:
            camera.stop_recording()
            return return_int

if __name__ == "__main__":
    serve(5000)
