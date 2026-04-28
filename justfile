# Event Management System - Build Recipes

cmake_gen:
    cmake -S. -B build -DCMAKE_BUILD_TYPE=Release

build: cmake_gen
    cmake --build build -j

debug: cmake_gen
    cmake -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build -j

run: build
    ./build/src/EventManagementSystem

help:
    @echo "Available commands:"
    @echo "   just build   - Configure and build the application"
    @echo "   just debug   - Build in debug mode"
    @echo "   just run     - Build and run the application"
    @echo "   just help    - Show this help message"
