CC = g++
CFLAGS = -std=c++11
LIBS = -linput -ludev
SRC = multitouch_reader.cpp
TARGET = multitouch_reader

.DEFAULT_GOAL := all

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
