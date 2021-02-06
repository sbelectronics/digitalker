import sys

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
        
if "-b" in sys.argv:
    for bank in sorted(vocab.keys()):
        words = vocab[bank]
        if words:
            sys.stdout.write(chr(0xFE) + chr(bank))
            for index in sorted(words.keys()):
                word = words[index]
                sys.stdout.write(chr(index) + chr(len(word)) + word)
    sys.stdout.write(chr(0xFF))

elif "-c" in sys.argv:
    l = 0
    for bank, words in vocab.items():
        if words:
            l = l + 2
            for word in words.values():
                l = l + len(word) + 2
    l = l + 1

    print "#define WORDS_BYTES %d" % l
    print "const uint8_t vocab[WORDS_BYTES] PROGMEM = {"
    for bank in sorted(vocab.keys()):
        words = vocab[bank]
        if words:
            print "  0xFE,\n  0x%02X," % bank
            for index in sorted(words.keys()):
                word = words[index]
                print "  0x%02X,\n  0x%02X," % (index, len(word))
                for ch in word:
                    print "  0x%02X, /* %s */" % (ord(ch), ch)
    print "  0xFF,"
    print "};"
else:
    from pprint import pprint

    print "vocab=",
    pprint(vocab)

    print "reverse=",
    pprint(reverse)

