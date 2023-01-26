from time import sleep
import board
from adafruit_motorkit import MotorKit

kit = MotorKit(i2c = board.I2C())

for i in range(1000):
	kit.stepper1.onestep()
	sleep(0.01)
