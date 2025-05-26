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

install-completion:
	cp tracker_completion.sh $$HOME/.tracker_completion.sh
	if [ -n "$$ZSH_VERSION" ]; then \
	  CONFIG_FILE="$$HOME/.zshrc"; \
	else \
	  CONFIG_FILE="$$HOME/.bashrc"; \
	fi; \
	grep -qxF 'source $$HOME/.tracker_completion.sh' $$CONFIG_FILE || echo 'source $$HOME/.tracker_completion.sh' >> $$CONFIG_FILE; \
	echo "Auto-completion script installed. Please restart your shell or run: source $$CONFIG_FILE"

clean:
	rm -f tracker
