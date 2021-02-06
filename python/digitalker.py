from smbpi.i2c_with_crc import I2CWithCrc, I2CError, ReceiveCRCError, ReceiveSizeError, ReceiveUninitializedError, CRCError, NoMoreRetriesError
from digitalker_base import DigitalkerBase
from digitalker_regs import REG_SPEECH_QUEUE_BANK, REG_SPEECH_QUEUE_WORD, REG_ECHO
import time
import vocab


class DigitalkerBoard(I2CWithCrc, DigitalkerBase):
    def __init__(self, bus=None, addr=0x5, pi=None, sdaPin=2, sclPin=3):
        I2CWithCrc.__init__(self, bus, addr, pi, sdaPin, sclPin)
        self.lastBank = None

    def sayWord(self, w):
        b = (w >> 8)
        w = (w & 0x0FF)

        if (b != self.lastBank):
            self.writeReg(REG_SPEECH_QUEUE_BANK, b)
            self.lastBank = b

        self.writeReg(REG_SPEECH_QUEUE_WORD, w)

    def echo(self, v):
        self.writeReg(REG_ECHO, v)
        readBack = self.readReg(REG_ECHO)
        if readBack != v:
            print "ERROR! Echo failed", v, readBack

def dumpWords(sb):
    lastLastWordOut = None
    while True:
        lastWordOut = sb.readSpeechLastWordOut()
        if (lastWordOut != lastLastWordOut):
            print lastWordOut
            lastLastWordOut = lastWordOut
        time.sleep(0.001)

def main():
    import smbus
    bus = smbus.SMBus(1)
    sb = DigitalkerBoard(bus=bus)

    sb.say("near")

    dumpWords(sb)

    while True:
        sb.dumpRegs()
        time.sleep(1)


if __name__ == "__main__":
    main()