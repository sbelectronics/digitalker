import select
import socket
import time
import vocab

from digitalker_base import DigitalkerBase


class DigitalkerTcp(DigitalkerBase):
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

    def drain(self):
        while True:
            ready, o, e = select.select([self.sock], [], [], 0.0)
            if not ready:
                return

            for s in ready:
                s.recv(1024)

    def readReg(self, num):
        self.drain()
        self.sock.send(chr(220 + num))

        ready, o, e = select.select([self.sock], [], [], 1)
        if not ready:
            return None

        # we should only get one byte back
        data = self.sock.recv(1024)
        return ord(data[-1])

    def sayWord(self, w):
        b = (w >> 8)
        w = (w & 0x0FF)

        if (b != self.lastBank):
            self.sock.send(chr(240 + b))
            self.lastBank = b

        self.sock.send(chr(w))

    def setVolume(self, v):
        self.sock.send(chr(219))
        self.sock.send(chr(v))


def main():
    sb = DigitalkerTcp(addr="esp-digitalker.lan:8888")

    sb.say("near")


if __name__ == "__main__":
    main()