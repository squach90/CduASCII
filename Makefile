CC = gcc
CFLAGS = -Wall -O2 -Iinclude
TARGET = cduascii
SRC = src/main.c
LIBS = -lm

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)
