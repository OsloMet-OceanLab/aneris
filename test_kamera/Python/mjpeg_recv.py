from cv2 import VideoCapture, imshow, destroyAllWindows, waitKey, error

class Video(VideoCapture):
	def __enter__(self):
		return self
	def __exit__(self, *args):
		self.release()

def main():
	try:
		with Video("http://localhost:5000/stream") as cap:
			if not cap.isOpened():
				raise Exception('Unable to open stream')
			while True:
				_, frame = cap.read()
				imshow('frame', frame)
				if waitKey(10) == 27:
					break
	except Exception as e:
		print(f'Exception: {str(e)}')
	except error as e: # opencv custom exceptions handling
		print(f'Exception: {str(e)}')
	finally:
		destroyAllWindows()
		print('Done')

if __name__ == "__main__":
	main()
