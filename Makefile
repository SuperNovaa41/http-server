TARGET = http
VPATH = src
CC = gcc

OBJ = main.o tcp.o http.o file.o string_helpers.o

$(TARGET): $(OBJ)
	mkdir -p build
	$(CC) -o $(TARGET) $(OBJ) -g
	mv $(TARGET) build/

main.o: tcp.h http.h 
tcp.o: tcp.h
http.o: http.h _defines.h string_helpers.h file.h
file.o: file.h
string_helpers.o: string_helpers.h

.PHONY: clean
clean:
	rm -rf build $(OBJ)
