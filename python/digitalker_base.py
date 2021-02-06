from digitalker_regs import *
import vocab

class DigitalkerBase:
    def __init__(self):
        pass

    def readSpeechLastWordOut(self):
        return self.readReg(REG_SPEECH_LAST_WORD_OUT)

    def readSpeechLastBankOut(self):
        return self.readReg(REG_SPEECH_LAST_BANK_OUT)        

    def readSpeechBufCount(self):
        return self.readReg(REG_SPEECH_BUF_COUNT)

    def readSpeechState(self):
        return self.readReg(REG_SPEECH_STATE)

    def say(self, v):
        if v in vocab.reverse:
            (bank, word) = vocab.reverse[v]
            self.sayWord(bank<<8 | word)        

    def dumpRegs(self):
        lastWordOut = self.readSpeechLastWordOut()
        lastBankOut = self.readSpeechLastBankOut()
        bufCount = self.readSpeechBufCount()
        state = self.readSpeechState()
        print "lastWord=%02X, lastBank=%02X, bufCount=%d, state=%d" % (lastWordOut, lastBankOut, bufCount, state)
        print ""
