#!/bin/bash

# Check if CMake is already configured
if [ ! -d "cmake-build-debug" ]; then
    echo "Creating build directory..."
    mkdir cmake-build-debug
fi

# Navigate to the build directory
cd cmake-build-debug || exit

# Run CMake and compile the project
echo "Configuring with CMake..."
cmake .. || { echo "CMake configuration failed"; exit 1; }

echo "Building the project..."
make || { echo "Build failed"; exit 1; }

# Run the executable if build is successful
echo "Running the game..."
./Minesweeper