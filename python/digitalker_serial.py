import serial
import vocab

from digitalker_regs import *
from digitalker_base import DigitalkerBase

class DigitalkerSerial(DigitalkerBase):
    def __init__(self, dev):
        self.dev = dev
        self.lastBank = None

        self.connect()

    def connect(self):
        self.serial = serial.Serial(self.dev, 9600)

    # NOTE: readReg untested
    def readReg(self, num):
        self.serial.reset_input_buffer()
        self.serial.write(chr(220 + num))

        self.serial.timeout = 1
        try:
            data = self.serial.read(size=1)
        except serial.SerialTimeoutException:
            return None

        if not data:
            return None

        return ord(data[-1]) 

    def sayWord(self, w):
        b = (w >> 8)
        w = (w & 0x0FF)

        if (b != self.lastBank):
            self.serial.write(chr(240 + b))
            self.lastBank = b

        self.serial.write(chr(w))

    def setVolume(self, v):
        self.serial.write([chr(219), chr(v)])

def main():
    sb = DigitalkerSerial(dev="/dev/ttyUSB0")

    sb.say("near")


if __name__ == "__main__":
    main()