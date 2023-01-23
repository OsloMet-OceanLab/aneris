import cv2 as cv
import socket


def main():
	HOST = "127.0.0.1"
	PORT = 5000
	cap = cv.VideoCapture(0, cv.CAP_V4L)
	if not cap.isOpened():
		print("Camera is unavailable")
		exit()
		
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.bind((HOST, PORT))
		s.listen()
		conn, addr = s.accept()
		with conn:
			while True:
				ret, frame = cap.read()
				
				if not ret:
					print("Can't receive stream")
					exit()
				
				gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
				conn.sendall(frame)

if __name__ == "__main__":
	main()
