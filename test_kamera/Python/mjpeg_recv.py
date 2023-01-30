from cv2 import VideoCapture, imshow, destroyAllWindows, waitKey
from sys import exit

def main():
	try:
		cap = VideoCapture("http://localhost:5000/stream")
		if not cap.isOpened():
			print('Unable to open stream')
			exit(1)
			
		while True:
			_, frame = cap.read()
			imshow('frame', frame)
			if waitKey(10) == 27:
				break
	except Exception as e:
		print(f'Exception: {str(e)}')
	finally:
		cap.release()
		destroyAllWindows()
		print('Done')

if __name__ == "__main__":
	main()
