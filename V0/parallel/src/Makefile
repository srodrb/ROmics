CC = icc
CFLAGS = -DDEBUG_MODE -std=c99
OPT    = -O3
CLIBS    = -lm -L /opt/OpenBLAS/lib -lopenblas 
INCLUDES =  -I /opt/OpenBLAS/include

ALL = main

all: $(ALL)

cca_seq.o: cca_par.c cca_par.h
	$(CC) $(OPT) $(CFLAGS) $^ -c

main.o: main.c
	$(CC) $(OPT) $(CFLAGS) $^ -c

main: main.o debug.h cca_par.o
	$(CC) $(OPT) $(FLAGS) $^ -o $@ $(CLIBS) $(INCLUDES)
	mv $@ bin/

.PHONY: clean
clean:
	rm -f *.o *.h.gch $(ALL)
