CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2

.PHONY: all clean test

all: fibonacci premiers

fibonacci: fibonacci.c
	$(CC) $(CFLAGS) -o fibonacci fibonacci.c

premiers: premiers.c
	$(CC) $(CFLAGS) -o premiers premiers.c

test: all
	./fibonacci
	./premiers

clean:
	rm -f fibonacci premiers fibonacci.exe premiers.exe
