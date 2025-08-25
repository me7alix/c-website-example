CC = gcc
CFLAGS = -std=c99 -Wall

build:
	mkdir -p build
	$(CC) $(CFLAGS) src/tmpls.c -o ./build/tmpls && ./build/tmpls
	$(CC) $(CFLAGS) src/main.c src/storage.c -o ./build/main -lsqlite3

run: build
	./build/main

clean:
	rm -rf build
