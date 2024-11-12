//
// Created by jtole on 7/24/2024.
//
#include <SFML/Graphics.hpp>
#include "tile.h"
#include "texturemanager.h"
#include <vector>
#include <random>
#include <fstream>
#include <map>
#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H
class GameBoard {
private:
    TextureManager textures;
    std::vector<std::vector<Tile*>> board;
    sf::Sprite face;
    sf::Sprite debug;
    sf::Sprite pause_play;
    sf::Sprite leaderboard;
    int number_mines;
    int mines_flagged;
    int number_rows;
    int number_columns;
public:
    GameBoard(int x, int y, int z) {
        for (int i = 0; i < y; i++) {
            std::vector<Tile*> row;
            row.reserve(x);
            for (int j = 0; j < x; j++) {
                row.push_back(new Tile(j, i));
            }
            board.push_back(row);
        }
        face.setTexture(textures.face_happy);
        face.setPosition((x * 16) - 32, 32 * (y + .5)); //NOLINT
        debug.setTexture(textures.debug);
        debug.setPosition((x * 32) - 304, 32 * (y + .5)); //NOLINT
        pause_play.setTexture(textures.pause);
        pause_play.setPosition((x * 32) - 240, 32 * (y + .5)); //NOLINT
        leaderboard.setTexture(textures.leaderboard);
        leaderboard.setPosition((x * 32) - 176, 32 * (y + .5)); //NOLINT
        number_mines = z;
        number_rows = y;
        number_columns = x;
        mines_flagged = 0;
    }
    std::string checkGameStatus() {
        minesFlagged();
        int tiles_revealed = 0;
        for (auto& row: board) {
            for (auto& tile: row) {
                if (tile->isMine() && !tile->isHidden())
                    return "Lose";
                else if (!tile->isMine() && !tile->isHidden()){
                    tiles_revealed++;
                }
            }
        }
        if (number_mines == mines_flagged && tiles_revealed == (number_rows * number_columns) - number_mines) {
            return "Win";
        }
        else return "Continue";
    };
    void minesFlagged() {
        int total_flagged = 0;
        for (auto& row: board) {
            for (auto& cell: row) {
                if (cell->isMine() && cell->isFlagged() && cell->isHidden())
                    total_flagged++;
            }
        }
        mines_flagged = total_flagged;
    }
    int tilesFlagged() {
        int total_flagged = 0;
        for (auto& row: board) {
            for (auto& cell: row) {
                if (cell->isFlagged())
                    total_flagged++;
            }
        }
        return total_flagged;
    }

    void drawToWindow(sf::RenderWindow& window, int time) {
        for (auto& row: board) {
            for (auto& column: row) {
                if (column->isMine() && !column->isHidden()) {
                    sf::Sprite temp(textures.tile_revealed);
                    temp.setPosition(column->getSprite().getPosition());
                    window.draw(temp);
                }
                window.draw(column->getSprite());
                if (!column->isHidden())
                    window.draw(column->getNumber());
                if (column->isFlagged())
                    window.draw(column->getFlag());
            }
        }
        std::string digit1 = std::to_string(number_mines - tilesFlagged());
        while (digit1.length() < 3) {
            digit1.insert(0, "0");
        }
        if (number_mines - tilesFlagged() < 0 && number_mines - tilesFlagged() > -10) {
            digit1[1] = '0';
            digit1[0] = '-';
        }
        else if (number_mines - tilesFlagged() < -99)
            digit1 = "-99";
        std::string digit2 = std::to_string(time);
        while (digit2.length() < 4) {
            digit2.insert(0, "0");
        }
        std::string minute = digit2.substr(0,2), seconds = digit2.substr(2,2);
        int i = 0;
        for (auto& digit: digit1) {
            int factor = (digit != '-') ? digit - '0' : 10;
            sf::IntRect rect(21 * factor, 0, 21, 32);
            sf::Sprite image(textures.digits, rect);
            image.setPosition(33 + (i++ * 21), 32 * (number_rows +.5) + 16); //NOLINT
            window.draw(image);
        }
        i = 0;
        for (auto& digit: minute) {
            int factor = digit - '0';
            sf::IntRect rect(21 * factor, 0, 21, 32);
            sf::Sprite image(textures.digits, rect);
            image.setPosition((number_columns * 32) - 97 + (i++ * 21), 32 * (number_rows +.5) + 16); //NOLINT
            window.draw(image);
        }
        i = 0;
        for (auto& digit: seconds) {
            int factor = digit - '0';
            sf::IntRect rect(21 * factor, 0, 21, 32);
            sf::Sprite image(textures.digits, rect);
            image.setPosition((number_columns * 32) - 54 + (i++ * 21), 32 * (number_rows +.5) + 16); //NOLINT
            window.draw(image);
        }
        window.draw(face);
        window.draw(debug);
        window.draw(pause_play);
        window.draw(leaderboard);
    }
    void leftClickedTile(int x, int y) {
        for (auto& row: board) {
            for (auto& tile: row) {
                sf::Sprite sprite = tile->getSprite();
                sprite.setTexture(textures.tile_revealed);
                float x1 = sprite.getPosition().x;
                float y1 = sprite.getPosition().y;
                float x2 = sprite.getLocalBounds().width;
                float y2 = sprite.getLocalBounds().height;
                if ((x >= x1 && x <= x1 +x2) && (y >= y1 && y <= y1 + y2) & !tile->isFlagged()) { //NOLINT
                    tile->Unreveal();
                    return;
                } //NOLINT
            }
        }
    };
    void generateBoard() {
        int num_mines = number_mines;
        while (num_mines > 0) {
            int x = rand() % number_columns; //NOLINT
            int y = rand() % number_rows; //NOLINT
            if (!board[y][x]->isMine()) {
                board[y][x]->setMine();
                num_mines--;
            }
            else{
                continue;
            }
        }
        setAdjacentMines();
    }
    void setAdjacentMines() {
        int min_col = 0, min_row = 0, max_col = board[0].size() - 1, max_row = board.size() - 1; //NOLINT
        for (int row = 0; row < board.size(); row++) {
            for (int column = 0; column < board[0].size(); column++) {
                std::vector<Tile*> adjacent;
                int C_left = column - 1, C_right = column + 1, R_top = row - 1, R_bottom = row + 1;
                if (C_left >= min_col)  // same row and left of tile
                    adjacent.push_back(board[row][C_left]);
                if (C_left >= min_col && R_top >= min_row) //upper left corner of tile
                    adjacent.push_back(board[R_top][C_left]);
                if (C_left >= min_col && R_bottom <= max_row) // bottom left corner of tile
                    adjacent.push_back(board[R_bottom][C_left]);
                if (R_top >= min_row) // same column and row above tile
                    adjacent.push_back(board[R_top][column]);
                if (R_bottom <= max_row) // same column and row below tile
                    adjacent.push_back(board[R_bottom][column]);
                if (C_right <= max_col) // same row and right of tile
                    adjacent.push_back(board[row][C_right]);
                if (C_right <= max_col && R_top >= min_row) // upper right corner of tile
                    adjacent.push_back(board[R_top][C_right]);
                if (C_right <= max_col && R_bottom <= max_row) //bottom right corner of tile
                    adjacent.push_back(board[R_bottom][C_right]);
                board[row][column]->setAdjacentMines(adjacent);
            }
        }
    }
    void rightClickedTile(int x, int y) {
        for (auto& row: board) {
            for (auto& tile: row) {
                sf::Sprite sprite = tile->getSprite();
                float x1 = sprite.getPosition().x;
                float y1 = sprite.getPosition().y;
                float x2 = sprite.getLocalBounds().width;
                float y2 = sprite.getLocalBounds().height;
                if ((x >= x1 && x <= x1 +x2) && (y >= y1 && y <= y1 + y2)) { //NOLINT
                    if (!tile->isFlagged()) {
                        tile->flagTile();
                    }
                    else {
                        tile->clearFlag();
                    }
                    return;
                } //NOLINT
            }
        }
    }
    void Debug(sf::RenderWindow& window) {
        for (auto& row: board) {
            for (auto &tile: row) {
                if (tile->isMine()) {
                    sf::Sprite mine(textures.mine);
                    mine.setPosition(tile->getSprite().getPosition());
                    window.draw(mine);
                }
            }
        }
    }
    void Restart(int x, int y, int z) {
        for (auto& row: board) {
            for (auto& tile: row) {

                delete tile;
                tile = nullptr;
            }
        }
        board.clear();
            for (int i = 0; i < y; i++) {
                std::vector<Tile *> row;
                row.reserve(x);
                for (int j = 0; j < x; j++) {
                    row.push_back(new Tile(j, i));
                }
                board.push_back(row);
            }
            generateBoard();
            face.setTexture(textures.face_happy);
            face.setPosition((x * 16) - 32, 32 * (y + .5)); //NOLINT
            debug.setTexture(textures.debug);
            debug.setPosition((x * 32) - 304, 32 * (y + .5)); //NOLINT
            pause_play.setTexture(textures.play);
            pause_play.setPosition((x * 32) - 240, 32 * (y + .5)); //NOLINT
            leaderboard.setTexture(textures.leaderboard);
            leaderboard.setPosition((x * 32) - 176, 32 * (y + .5)); //NOLINT
            number_mines = z;
            number_rows = y;
            number_columns = x;
    }
    bool isDebugButton(int x, int y) {
        float x1 = debug.getPosition().x;
        float y1 = debug.getPosition().y;
        float x2 = debug.getLocalBounds().width;
        float y2 = debug.getLocalBounds().height;
        if ((x >= x1 && x <= x1 + x2) && (y >= y1 && y <= y1 + y2)) { //NOLINT
            return true;
        }
        else return false;
    }
    bool isPausePlayButton(int x, int y) {
        float x1 = pause_play.getPosition().x;
        float y1 = pause_play.getPosition().y;
        float x2 = pause_play.getLocalBounds().width;
        float y2 = pause_play.getLocalBounds().height;
        if ((x >= x1 && x <= x1 + x2) && (y >= y1 && y <= y1 + y2)) { //NOLINT
            return true;
        } else return false;
    }
    bool isLeaderboardButton(int x, int y) {
        float x1 = leaderboard.getPosition().x;
        float y1 = leaderboard.getPosition().y;
        float x2 = leaderboard.getLocalBounds().width;
        float y2 = leaderboard.getLocalBounds().height;
        if ((x >= x1 && x <= x1 + x2) && (y >= y1 && y <= y1 + y2)) { //NOLINT
            return true;
        } else return false;
    }
    bool isFaceButton(int x, int y) {
        float x1 = face.getPosition().x;
        float y1 = face.getPosition().y;
        float x2 = face.getLocalBounds().width;
        float y2 = face.getLocalBounds().height;
        if ((x >= x1 && x <= x1 + x2) && (y >= y1 && y <= y1 + y2)) { //NOLINT
            return true;
        } else return false;
    }
    void switchPlayMode(bool mode,sf::RenderWindow& window) {
        if (mode)
            pause_play.setTexture(textures.play);
        else
            pause_play.setTexture(textures.pause);
        window.draw(pause_play);
    }
    void switchFaceMode(bool condition) {
        if (condition)
            face.setTexture(textures.face_win);
        else
            face.setTexture(textures.face_lose);
    }
    void displayPausedBoard(sf::RenderWindow& window) {
        for (auto& row: board) {
            for (auto &tile: row) {
                window.draw(tile->pauseTile());
            }
        }
    }
    void displaySolution(sf::RenderWindow& window) {
        for (auto& row: board) {
            for (auto& tile: row) {
                sf::Sprite temp = tile->getSprite();
                if (tile->isMine()) {
                    temp.setTexture(textures.tile_revealed);
                    window.draw(temp);
                    temp.setTexture(textures.mine);
                    window.draw(temp);
                }
                else if (!tile->isHidden()) {
                    temp.setTexture(textures.tile_revealed);
                    window.draw(temp);
                    window.draw(tile->getNumber());
                }
                else if (tile->isFlagged() && tile->isHidden()) {
                    window.draw(temp);
                    window.draw(tile->getFlag());
                }
            }
        }
    }
    void updateLeaderBoard(sf::String& variable, int time) {
        std::string user = variable;
        std::string line;
        std::string data;
        std::ifstream file("../files/leaderboard.txt");
        std::map<int, std::string> information;
        int i = 0;
        bool added = false;
        while (getline(file, line) && i < 5) {
            line.erase(2, 1);
            int number = stoi(line);
            if (!added) {
                if (time < number) {
                    information[time] = user + '*';
                    added = true;
                    i++;
                }
            }
            line.end();
            line.erase(0, line.find(", ") + 2);
            information[number] = line;
            i++;
        }
        file.close();
        std::map<int, std::string>::iterator iter;
        std::ofstream o_file("../files/leaderboard.txt", std::ios_base::trunc);
        for (iter = information.begin(); iter != information.end(); iter++) {
            std::string f_number = std::to_string(iter->first);
            while (f_number.length() < 4)
                f_number = "0" + f_number;
            f_number.insert(2, ":");
            o_file << f_number + ", " + iter->second +"\n";
        }
        o_file.close();
    };
};
#endif //MINESWEEPER_BOARD_H
