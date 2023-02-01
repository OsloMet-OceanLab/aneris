import RPi.GPIO as gpio
import time

ledPin = 4

gpio.setmode(gpio.BCM)
gpio.setup(ledPin, gpio.OUT)

gpio.output(ledPin, gpio.LOW)

try:
	while True:
		choice = int(input("0 for off, 1 for on: "))
		if choice == 0:
			gpio.output(ledPin, gpio.LOW)
		elif choice == 1:
			gpio.output(ledPin, gpio.HIGH)
		else:
			print("Invalid choice")
except:
	print("Done")
