#!/bin/bash
# Build script for SDL2 Proof of Concept

echo "Building SDL2 Proof of Concept for WatchySim..."

# Check if SDL2 is installed
if ! pkg-config --exists sdl2; then
    echo "Error: SDL2 not found. Please install it with:"
    echo "  sudo apt-get install libsdl2-dev"
    exit 1
fi

# Compile the proof of concept
g++ -o sdl2_poc sdl2_poc.cpp `sdl2-config --cflags --libs` -std=c++11

if [ $? -eq 0 ]; then
    echo "Build successful! Run with: ./sdl2_poc"
    echo ""
    echo "Controls:"
    echo "  - ESC or close window: Exit"
    echo "  - SPACE: Redraw test pattern"
    echo "  - C: Clear display"
    echo "  - Left click: Draw at mouse position"
else
    echo "Build failed!"
    exit 1
fi