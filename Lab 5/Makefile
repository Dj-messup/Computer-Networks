CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

all: getipaddr

getipaddr: getipaddr.c
	$(CC) $(CFLAGS) getipaddr.c -o getipaddr

clean:
	rm -f getipaddr getipaddr.out

.PHONY: all clean
