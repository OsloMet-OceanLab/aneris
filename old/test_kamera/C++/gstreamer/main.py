import cv2

cap = cv2.VideoCapture("tcp://localhost:5000")
if not cap.isOpened():
	exit(-1)
	
fps = cap.get(cv2.CAP_PROP_FPS)
wait_ms = int(1000/fps)
print("FPS: ", fps)

while True:
	ret, frame = cap.read()
	cv2.imshow('frame', frame)
	if cv.waitKey(10) == 27:
			break
			
cap.release()
cv2.destroyAllWindows()
