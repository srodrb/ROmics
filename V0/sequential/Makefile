CC = gcc
CFLAGS = -O3 -std=c99 -Wall
CLIBS  = -lm -L /usr/lib64/atlas/ -latlas

INCLUDES = -I /usr/include/cblas

INC_DIR = headers
SRC_DIR = src

ALL = cca

all: $(ALL)

cca.o: src/cca_seq.c headers/cca_seq.h
	$(CC) $(CFLAGS) $^ -c
	
cca: src/main.c src/cca.o
	$(CC) $(CFLAGS) $^ -o $@ $(CLIBS)
