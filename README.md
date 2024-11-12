# Minesweeper Game in C++ with SFML

This project is a classic implementation of the Minesweeper game, developed in C++ and powered by SFML (Simple and Fast Multimedia Library) for graphics and user interface. The game delivers an engaging experience with a sleek, minimalist design and smooth animations, allowing users to test their problem-solving skills and logic as they reveal hidden mines scattered across the board.

## Features

- **Multiple Difficulty Levels**: Choose from various board sizes and mine counts to adjust the game's difficulty.
- **User-Friendly Interface**: Intuitive, easy-to-navigate UI built with SFML for smooth gameplay and responsive feedback.
- **Flagging System**: Mark suspected mines with flags, helping players keep track of potentially dangerous spots.
- **Timer**: A built-in timer records the duration of each game, encouraging players to improve their completion time.
- **Win/Loss Notification**: Clear notifications for winning or losing, with the option to reset and try again.

## How to Play

1. **Objective**: Clear the board without uncovering any mines.
2. **Revealing Tiles**: Click on any tile to reveal it. If it contains a mine, the game is over!
3. **Flagging Mines**: Right-click to place a flag on tiles you suspect contain mines.
4. **Hints**: Numbered tiles indicate how many adjacent tiles contain mines, helping you strategize and avoid mines.

## Getting Started

### Prerequisites

- **C++ Compiler**: Compatible with C++11 or later.
- **SFML Library**: Ensure SFML is installed on your system. [Download SFML](https://www.sfml-dev.org/download.php)

### Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/minesweeper-sfml
   cd minesweeper-sfml

g++ -std=c++11 -o Minesweeper main.cpp -lsfml-graphics -lsfml-window -lsfml-system

./Minesweeper
