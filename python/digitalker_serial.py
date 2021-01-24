import serial
import vocab


class DigitalkerSerial:
    def __init__(self, dev):
        self.dev = dev
        self.lastBank = None

        self.connect()

    def connect(self):
        self.serial = serial.Serial(self.dev, 9600);

    def sayWord(self, w):
        b = (w >> 8)
        w = (w & 0x0FF)

        if (b != self.lastBank):
            print "serwrite", 240+b
            self.serial.write(chr(240 + b))
            self.lastBank = b

        print "serwrite", w
        self.serial.write(chr(w))

    def say(self, v):
        if v in vocab.reverse:
            (bank, word) = vocab.reverse[v]
            self.sayWord(bank<<8 | word)

    def dumpRegs(self):
        pass


def main():
    sb = DigitalkerSerial(dev="/dev/ttyUSB0")

    sb.say("near")


if __name__ == "__main__":
    main()