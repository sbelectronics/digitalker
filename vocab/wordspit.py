bankNum = 0
vocab = {}
reverse = {}
for FN in ["words-bank0.txt", "words-bank1.txt", "words-bank2.txt", "words-bank3.txt",
           "words-bank4.txt", "words-bank5.txt", "words-bank6.txt", "words-bank7.txt"]:
    vocab[bankNum] = {}

    lines = open(FN).readlines()
    lines = [x.strip() for x in lines]
    lines = [x for x in lines if x]

    index = 0
    for line in lines:
        if ":" in line:
          (indexStr, line) = line.split(":")
          index = int(indexStr.strip())
          line = line.strip()
        vocab[bankNum][index] = line

        if line not in reverse:
            reverse[line] = (bankNum, index)

        index+=1

    bankNum += 1
        
from pprint import pprint

print "vocab=",
pprint(vocab)

print "reverse=",
pprint(reverse)

