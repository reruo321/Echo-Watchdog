CC=gcc
CFLAGS=-g -Wall -O2
LDFLAGS=-lpthread
TARGET=EchoWatchdog
SOURCES=main.c ewdshell.c
OBJECTS=$(SOURCES:.c=.o)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.c ewdshell.h
	$(CC) $(CFLAGS) -c $< -o $@

ewdshell.o: ewdshell.c ewdshell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

clean_all:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean clean_all
