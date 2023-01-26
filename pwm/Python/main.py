from time import sleep
import board
from adafruit_motorkit import MotorKit
kit = MotorKit()

kit.motor1.throttle = 1.0
kit.motor2.throttle = 1.0
sleep(0.5)
kit.motor1.throttle = 0
kit.motor2.throttle = 0
