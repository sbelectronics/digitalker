all: vocab-python vocab-bin vocab-bin-inc

vocab-python:
	python ./wordspit.py > vocab.py
	cp vocab.py ../python/

vocab-bin:
	python ./wordspit.py -b > vocab.bin

vocab-bin-inc:
	python ./wordspit.py -c > vocab.bin.inc
