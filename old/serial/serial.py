import time
import serial

ser = serial.Serial(
        port='/dev/ttyS0',
        baudrate = 9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=1
)

while True:
        #ser.write('Write counter: %d \n'%(counter))
        #time.sleep(1)
        #counter += 1
        x = ser.readline()
        print(x)
#	ser.write("received: " + x)
