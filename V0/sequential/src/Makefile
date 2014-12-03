CC = gcc
CFLAGS = -DDEBUG_MODE -std=c99 -O3
OPT    = -O3 -msse3
CLIBS    = -lm  -L /usr/lib64/atlas-sse3 -llapack -lblas
INCLUDES =  -I /usr/include/atlas

ALL = main

all: $(ALL)

cca_seq.o: cca_seq.c cca_seq.h
	$(CC) $(OPT) $(CFLAGS) $^ -c
	
main: main.c debug.h cca_seq.o
	$(CC) $(OPT) $(CFLAGS) $^ -o $@ $(INCLUDES) $(CLIBS)

clean:
	rm -f *.o *.h.gch $(ALL)
