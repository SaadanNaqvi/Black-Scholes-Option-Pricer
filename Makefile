# ---- Simple Makefile for ad-hoc builds with SFML 3 ----
CXX := clang++

# Strip any trailing whitespace/newlines from brew --prefix
HB  := $(strip $(shell brew --prefix))

CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -I$(HB)/include
LDFLAGS  := -L$(HB)/lib -lsfml-graphics -lsfml-window -lsfml-system \
            -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL \
            -Wl,-rpath,$(HB)/lib

# Pass files at call time:
#   make build SRCS="src/main-dashboard.cpp src/Dashboard.cpp" OUT=dashboard
#   make run   SRCS="src/main-dashboard.cpp src/Dashboard.cpp" OUT=dashboard
SRCS ?= src/main.cpp
OUT  ?= app

.PHONY: build run clean print

build:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(OUT) $(LDFLAGS)

run: build
	./$(OUT)

clean:
	rm -f $(OUT)

print:
	@echo HB='$(HB)'
	@echo CXXFLAGS='$(CXXFLAGS)'
	@echo LDFLAGS='$(LDFLAGS)'
