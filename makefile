CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude

SRC := $(filter-out src/parser-expand.c,$(wildcard src/*.c))

TARGET=shellforge2

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -lreadline -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
