//
// Created by jtole on 7/24/2024.
//
#include <SFML/Graphics.hpp>
#ifndef MINESWEEPER_TILEMANAGER_H
#define MINESWEEPER_TILEMANAGER_H
struct TextureManager {
    sf::Texture tile_hidden;
    sf::Texture tile_revealed;
    sf::Texture mine;
    sf::Texture flag;
    sf::Texture num_1;
    sf::Texture num_2;
    sf::Texture num_3;
    sf::Texture num_4;
    sf::Texture num_5;
    sf::Texture num_6;
    sf::Texture num_7;
    sf::Texture num_8;
    sf::Texture face_happy;
    sf::Texture face_win;
    sf::Texture face_lose;
    sf::Texture digits;
    sf::Texture debug;
    sf::Texture pause;
    sf::Texture play;
    sf::Texture leaderboard;

    TextureManager() {
        tile_hidden.loadFromFile("../files/images/tile_hidden.png");
        tile_revealed.loadFromFile("../files/images/tile_revealed.png");
        mine.loadFromFile("../files/images/mine.png");
        flag.loadFromFile("../files/images/flag.png");
        num_1.loadFromFile("../files/images/number_1.png");
        num_2.loadFromFile("../files/images/number_2.png");
        num_3.loadFromFile("../files/images/number_3.png");
        num_4.loadFromFile("../files/images/number_4.png");
        num_5.loadFromFile("../files/images/number_5.png");
        num_6.loadFromFile("../files/images/number_6.png");
        num_7.loadFromFile("../files/images/number_7.png");
        num_8.loadFromFile("../files/images/number_8.png");
        face_happy.loadFromFile("../files/images/face_happy.png");
        face_win.loadFromFile("../files/images/face_win.png");
        face_lose.loadFromFile("../files/images/face_lose.png");
        digits.loadFromFile("../files/images/digits.png");
        debug.loadFromFile("../files/images/debug.png");
        pause.loadFromFile("../files/images/pause.png");
        play.loadFromFile("../files/images/play.png");
        leaderboard.loadFromFile("../files/images/leaderboard.png");
    }
};
#endif //MINESWEEPER_TILEMANAGER_H
