TARGET = http
VPATH = src
CC = gcc

OBJ = main.o tcp.o http.o file.o

$(TARGET): $(OBJ)
	mkdir -p build
	gcc -o $(TARGET) $(OBJ)
	mv $(TARGET) build/

main.o: tcp.h http.h file.h
tcp.o: tcp.h
http.o: http.h
file.o: file.h

.PHONY: clean
clean:
	rm -rf build $(OBJ)
