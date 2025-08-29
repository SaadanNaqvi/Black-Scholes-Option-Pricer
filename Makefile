.PHONY: all build run clean

all: build

build:
	rm -rf build
	cmake -S . -B build
	cmake --build build

run: build
	./build/main

clean:
	rm -rf build
