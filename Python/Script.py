import time
import serial
import sys

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=115200
)

ser.isOpen()

cmd = sys.argv[1]
ser.write(cmd)
ser.write("\r\n")
