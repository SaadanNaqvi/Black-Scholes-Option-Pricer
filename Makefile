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


user: main-user.cpp Stocks.cpp Portfolio.cpp User.cpp
	g++ main-user.cpp Stocks.cpp Portfolio.cpp User.cpp
	./a.out
