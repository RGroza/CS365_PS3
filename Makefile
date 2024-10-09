CC = gcc
CFLAGS = -g -Wall -Wextra -O0

TARGET = exploring
SRC = exploring.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean