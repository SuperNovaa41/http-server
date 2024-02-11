TARGET = http
VPATH = src
CC = gcc

OBJ = main.o tcp.o http.o file.o

$(TARGET): $(OBJ)
	mkdir -p build
	$(CC) -o $(TARGET) $(OBJ)
	mv $(TARGET) build/

main.o: tcp.h http.h file.h
tcp.o: tcp.h
http.o: http.h _defines.h
file.o: file.h

.PHONY: clean
clean:
	rm -rf build $(OBJ)
