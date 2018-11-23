import time
import serial
import sys

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB1',
    baudrate=115200
)

ser.isOpen()

cmd = sys.argv[1]
state = sys.argv[2]

if (cmd == "light"):
    if (state == "on"):
        ser.write("LAMP_ON\n")
    else:
        ser.write("LAMP_OFF\n")