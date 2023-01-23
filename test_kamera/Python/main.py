import cv2 as cv

def main():
	cap = cv.VideoCapture(0, cv.CAP_V4L)
	if not cap.isOpened():
		print("Camera is unavailable")
		exit()
		
	while True:
		ret, frame = cap.read()
		
		if not ret:
			print("Can't receive stream")
			exit()
		
		gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
		
		cv.imshow('frame', frame)
		
		if cv.waitKey(10) == 27:
			break

if __name__ == "__main__":
	main()
