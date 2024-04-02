CC=g++
CFLAGS=-O3 -Wall

SOURCES=$(wildcard cpp/*.cpp)
OBJECTS=$(patsubst cpp/%.cpp, cpp/%.o, $(SOURCES))

.PHONY: clean

all: lop

lop: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o lop

clean:
	rm -f cpp/*~ cpp/*.o lop