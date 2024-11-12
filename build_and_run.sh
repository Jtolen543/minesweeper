#!/bin/bash

# Remove the build directory if it exists
#if [ -d "cmake-build-debug" ]; then
#    echo "Removing existing build directory..."
#    rm -rf cmake-build-debug
#fi

# Create the build directory
echo "Creating build directory..."
mkdir cmake-build-debug
cd cmake-build-debug || exit

# Run CMake to configure the project
echo "Configuring with CMake..."
cmake .. || { echo "CMake configuration failed"; exit 1; }

# Build the project
echo "Building the project..."
make || { echo "Build failed"; exit 1; }

# Run the executable if the build is successful
echo "Running the game..."
./Minesweeper