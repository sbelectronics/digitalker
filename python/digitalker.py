from smbpi.i2c_with_crc import I2CWithCrc, I2CError, ReceiveCRCError, ReceiveSizeError, ReceiveUninitializedError, CRCError, NoMoreRetriesError
import time
import vocab

REG_RESET = 0
REG_ECHO = 1
REG_SPEECH_LAST_WORD_OUT = 10
REG_SPEECH_BUF_COUNT = 11
REG_SPEECH_STATE = 12
REG_SPEECH_QUEUE_BANK = 13
REG_SPEECH_QUEUE_WORD = 14
REG_SPEECH_LAST_BANK_OUT = 15

SPEECH_INIT =             0x00
SPEECH_BUSY =             0x01
SPEECH_IDLE =             0x02
SPEECH_BUSYWAIT =         0x03
SPEECH_ACKED =            0x04

class DigitalkerBoard(I2CWithCrc):
    def __init__(self, bus=None, addr=0x5, pi=None, sdaPin=2, sclPin=3):
        I2CWithCrc.__init__(self, bus, addr, pi, sdaPin, sclPin)
        self.lastBank = None

    def readSpeechLastWordOut(self):
        return self.readReg(REG_SPEECH_LAST_WORD_OUT)

    def readSpeechLastBankOut(self):
        return self.readReg(REG_SPEECH_LAST_BANK_OUT)        

    def readSpeechBufCount(self):
        return self.readReg(REG_SPEECH_BUF_COUNT)

    def readSpeechState(self):
        return self.readReg(REG_SPEECH_STATE)

    def sayWord(self, w):
        b = (w >> 8)
        w = (w & 0x0FF)

        if (b != self.lastBank):
            self.writeReg(REG_SPEECH_QUEUE_BANK, b)
            self.lastBank = b

        self.writeReg(REG_SPEECH_QUEUE_WORD, w)

    def say(self, v):
        if v in vocab.reverse:
            (bank, word) = vocab.reverse[v]
            self.sayWord(bank<<8 | word)

    def echo(self, v):
        self.writeReg(REG_ECHO, v)
        readBack = self.readReg(REG_ECHO)
        if readBack != v:
            print "ERROR! Echo failed", v, readBack

    def dumpRegs(self):
        lastWordOut = self.readSpeechLastWordOut()
        lastBankOut = self.readSpeechLastBankOut()
        bufCount = self.readSpeechBufCount()
        state = self.readSpeechState()
        print "lastWord=%02X, lastBank=%02X, bufCount=%d, state=%d" % (lastWordOut, lastBankOut, bufCount, state)
        print ""

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