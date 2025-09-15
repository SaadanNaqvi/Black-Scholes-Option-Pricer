# --- Simple SFML Makefile (SFML 3 + pkg-config) ---
CXX      := clang++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude $(shell pkg-config --cflags sfml-graphics)
LDFLAGS  := $(shell pkg-config --libs sfml-graphics)

# Usage:
#   make build SRCS="src/main.cpp src/Dashboard.cpp" OUT=dashboard
#   make run   SRCS="src/main.cpp src/Dashboard.cpp" OUT=dashboard
# Defaults:
SRCS ?= src/main.cpp
OUT  ?= app

.PHONY: build run clean

build:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(OUT) $(LDFLAGS)

run: build
	./$(OUT)

clean:
	rm -f $(OUT)
