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
	src/Dropdown.cpp \
	src/CSVData.cpp	\
	src/DatePicker.cpp \
	src/BlackScholesEquation.cpp \
	src/BSVarCalc.cpp 

RAYLIB_PATH:=/raylib

CXXFLAGSL := -std=c++17 -Wall -Isrc -Iinclude -Iraylib/src
LIBSL     := -L$(RAYLIB_PATH)/build/raylib -lraylib -lm -lpthread -ldl -lGL
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

dashboardL:
	@echo "Building dashboard..."
	g++ $(CXXFLAGSL) $(DASHBOARD_SRCS) -o dashboardL.out $(LIBSL)
	@echo "Running dashboard..."
	./dashboardL.out

stockTest:
	@echo "Building stock..."
	g++ $(CXXFLAGS) src/CSVData.cpp src/Stocks.cpp src/Order.cpp src/CSVData.cpp src/main-stock.cpp -o stock.out
	@echo "Running stock..."
	./stock.out

writeTest:
	@echo "Building stock..."
	g++ $(CXXFLAGS)  src/Portfolio.cpp src/User.cpp src/Stocks.cpp src/Order.cpp src/CSVData.cpp src/main-write.cpp -o write.out
	@echo "Running stock..."
	./write.out


clean:
	rm -f *.out
	@echo "🧹 Clean complete"



