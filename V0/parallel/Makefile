CC = gcc
CFLAGS = -DDEBUG_MODE -std=c99
OPT    = -O3
CLIBS    = -lm -L /opt/OpenBLAS/lib -lopenblas 
INCLUDES =  -I /opt/OpenBLAS/include

ALL = main

all: $(ALL)

cca_seq.o: src/cca_par.c headers/cca_par.h
	$(CC) $(OPT) $(CFLAGS) $^ -c
	
debug.o: src/debug.c headers/debug.h
	$(CC) $(OPT) $(CFLAGS) $^ -c	

main.o: src/main.c
	$(CC) $(OPT) $(CFLAGS) $^ -c

main: src/main.o src/debug.o src/cca_par.o
	$(CC) $(OPT) $(FLAGS) $^ -o $@ $(CLIBS) $(INCLUDES)
	mv $@ bin/

.PHONY: clean
clean:
	rm -f src/*.o src/*.h.gch bin/$(ALL)
