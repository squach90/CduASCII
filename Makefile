CC = gcc
CFLAGS = -Wall -O2 -Iinclude
LDFLAGS = -lm
SRC = src/main.c src/file_browser.c
TARGET = cduascii

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean