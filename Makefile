CC=clang
CFLAGS=-Wall -std=c99 -g
LDFLAGS=-lcrypt -lrt -pthread

all: cracker

cracker: cracker.c
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	
clean:
	rm -rf cracker
