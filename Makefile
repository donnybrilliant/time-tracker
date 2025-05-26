# Makefile

CC = gcc
CFLAGS = -I/usr/include -Wall -Wextra -g
LDFLAGS = -lsqlite3
SRC = src/main.c src/db.c
OUT = tracker

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f tracker tracker.db