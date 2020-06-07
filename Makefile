CC=g++
OBJECTS=BitStream.o Node.o HuffmanTree.o

all: compress uncompress

.cc.o:
	$(CC) -O3 -c $<

compress: $(OBJECTS) compress.cc
	$(CC) -o $@ $@.cc $(OBJECTS)

uncompress: $(OBJECTS) uncompress.cc
	$(CC) -o $@ $@.cc $(OBJECTS)

clean:
	rm -f $(OBJECTS) compress uncompress

BitStream.o: BitStream.h
BitStreamFile.o: BitStreamFile.h BitStream.h
Node.o: Node.h BitStream.h
HuffmanTree.o: Node.h HuffmanTree.h BitStream.h IsLessPointerPredicate.h
