import cv2 as cv
import socket
import numpy as np

def main():
	HOST = "127.0.0.1"
	PORT = 5000
		
	with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		s.connect((HOST, PORT))
		frame = np.zeros((640, 480, 3), dtype = "uint8")
		while True:
			frame = s.recv(921600)
			cv.imshow('frame', frame)
		
			if cv.waitKey(10) == 27:
				break

if __name__ == "__main__":
	main()
