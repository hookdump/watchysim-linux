# Makefile for WatchySim SDL2 Linux port

CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -I. -I.. -DWATCHY_SIM `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

# Object files
OBJS = WatchySim_SDL2.o \
       Watchy_SDL2.o \
       GraphicsAdapter_SDL2.o \
       arduino/Common.o \
       arduino/String.o \
       arduino/dtostrf.o

# Target executable
TARGET = WatchySim

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files
WatchySim_SDL2.o: WatchySim_SDL2.cpp Watchy_SDL2.h GraphicsAdapter.h
	$(CXX) $(CXXFLAGS) -c WatchySim_SDL2.cpp

Watchy_SDL2.o: Watchy_SDL2.cpp Watchy_SDL2.h GraphicsAdapter.h
	$(CXX) $(CXXFLAGS) -c Watchy_SDL2.cpp

GraphicsAdapter_SDL2.o: GraphicsAdapter_SDL2.cpp GraphicsAdapter.h
	$(CXX) $(CXXFLAGS) -c GraphicsAdapter_SDL2.cpp

arduino/Common.o: arduino/Common.cpp arduino/Common.h
	$(CXX) $(CXXFLAGS) -c arduino/Common.cpp -o arduino/Common.o

arduino/String.o: arduino/String.cpp arduino/String.h
	$(CXX) $(CXXFLAGS) -c arduino/String.cpp -o arduino/String.o

arduino/dtostrf.o: arduino/dtostrf.cpp arduino/dtostrf.h
	$(CXX) $(CXXFLAGS) -c arduino/dtostrf.cpp -o arduino/dtostrf.o


# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run