from cv2 import VideoCapture, imshow, destroyAllWindows, waitKey

import cv2 # temporary, better to just import individual classes/functions

class Video(cv2.VideoCapture):
	def __enter__(self):
		return self
	def __exit__(self, *args):
		self.release()

def main():
	try:
		with Video("http://192.168.2.2/stream") as cap:
			if not cap.isOpened():
				raise Exception('Unable to open stream')

			while True:
				_, frame = cap.read()
				
				# below is a code example to find squares from https://www.tutorialspoint.com/how-to-detect-a-rectangle-and-square-in-an-image-using-opencv-python

				gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
				ret, thresh = cv2.threshold(gray, 50, 255, 0)
				contours, hierarchy = cv2.findContours(thresh, 1, 2)

				print("Number of contours detected:", len(contours))

				for cnt in contours:
					x1, y1 = cnt[0][0]
					approx = cv2.approxPolyDP(cnt, 0.01 * cv2.arcLength(cnt, True), True)

					if len(approx) == 4:
						x, y, w, h = cv2.boundingRect(cnt)
						ratio = float(w)/h

						if 0.9 <= ratio <= 1.1:
							frame = cv2.drawContours(frame, [cnt], -1, (0, 255, 255), 3)
							cv2.putText(frame, 'Square', (x1, y1), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)

						else:
							cv2.putText(frame, 'Rectangle', (x1, y1), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
							frame = cv2.drawContours(frame, [cnt], -1, (0,255,0), 3)

				imshow('frame', frame)
				if waitKey(10) == 27:
					break
						
	except Exception as e:
		print(f'Exception: {str(e)}')
	finally:
		destroyAllWindows()
		print('Done')

if __name__ == "__main__":
	main()
