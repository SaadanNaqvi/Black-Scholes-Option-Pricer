CXXFLAGS  := -std=c++17 -Wall -Isrc -Iinclude -I/opt/homebrew/include
LIBS      := -L/opt/homebrew/lib -lraylib \
              -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

DASHBOARD_SRCS := \
	src/Graphics.cpp \
	src/main-graphics.cpp \
	src/Button.cpp \
	src/Bar.cpp \
	src/Line.cpp \
	src/Text.cpp \
	src/User.cpp \
	src/Portfolio.cpp \
	src/Order.cpp \
	src/Stocks.cpp \
	src/Position.cpp \

# Default target
all:
	@echo "Usage:"
	@echo "  make dashboard   → build & run dashboard"
	@echo "  make clean       → remove old builds"


dashboard:
	@echo "Building dashboard..."
	g++ $(CXXFLAGS) $(DASHBOARD_SRCS) -o dashboard.out $(LIBS)
	@echo "Running dashboard..."
	./dashboard.out


clean:
	rm -f *.out
	@echo "🧹 Clean complete"


