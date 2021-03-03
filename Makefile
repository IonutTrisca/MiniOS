CC = gcc
CFLAGS = -g -Wall -O3 -Wextra
HEADERS = header.h
FUNCTIONS = tema2.c functii_stiva.c functii_coada.c citire_comenzi.c functii_procese.c functii_auxiliare.c functii_lista.c
EXEC = tema2
IN = in_file
OUT = out_file

.PHONY = default build clean

default: build

build: $(EXEC)

$(EXEC): $(FUNCTIONS) $(HEADERS)
	$(CC) $(CFLAGS) -o $(EXEC) $(HEADERS) $(FUNCTIONS)

run: build
	./$(EXEC) $(IN) $(OUT)

clean:
	-rm -f $(EXEC) *.out

