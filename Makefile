CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lsqlite3
SRC = src/main.c src/db.c src/project.c src/task.c
OUT = tracker

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

install: all
	mkdir -p /usr/local/bin
	cp tracker /usr/local/bin/tracker

clean:
	rm -f tracker
