import socket
import time
import vocab


class DigitalkerTcp:
    def __init__(self, addr):
        if ":" not in addr:
            raise "No colon in address"

        self.host, self.port = addr.split(":")
        self.port = int(self.port)
            
        self.lastBank = None

        self.connect()

    def connect(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))

    def sayWord(self, w):
        b = (w >> 8)
        w = (w & 0x0FF)

        if (b != self.lastBank):
            self.sock.send(chr(240 + b))
            self.lastBank = b

        self.sock.send(chr(w))

    def say(self, v):
        if v in vocab.reverse:
            (bank, word) = vocab.reverse[v]
            self.sayWord(bank<<8 | word)

    def dumpRegs(self):
        pass


def main():
    sb = DigitalkerTcp(addr="esp-digitalker.lan:8888")

    sb.say("near")


if __name__ == "__main__":
    main()