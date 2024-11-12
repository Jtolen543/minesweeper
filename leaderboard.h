//
// Created by jtole on 7/26/2024.
//
#ifndef MINESWEEPER_LEADERBOARD_H
#define MINESWEEPER_LEADERBOARD_H
#include <map>
#include <SFML/Graphics.hpp>
#include <string>
#include "fstream"


class LeaderBoard {
private:
    std::map<int, std::string> players;
    sf::Text leader;
    sf::Text contents;
    std::ifstream i_file;
    std::ofstream o_file;
public:
    LeaderBoard(sf::RenderWindow& window, float width, float height, const sf::Font& font) {
        std::string line;
        std::string data;
        i_file.open("../files/leaderboard.txt");
        int i = 1;
        while (getline(i_file, line)) {
            int index = line.find(", "); //NOLINT
            line.insert(index, "\t");
            line.erase(index+1,2);
            data += std::to_string(i++) + ".\t" + line + "\n\n";
        }
        sf::Text leaderboard("LEADERBOARD", font, 20);
        leaderboard.setFillColor(sf::Color::White);
        leaderboard.setStyle(sf::Text::Underlined);
        setText(leaderboard, width / 2, height / 2 - 120);
        sf::Text content(data, font, 18);
        setText(content, width / 2, height / 2 + 20);
        content.setFillColor(sf::Color::White);
        leader = leaderboard;
        contents = content;
    }
    void setText(sf::Text &text, float x, float y){
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,
                         textRect.top + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }
    void writeText(sf::RenderWindow& window) {
        window.draw(leader);
        window.draw(contents);
    }
};

#endif //MINESWEEPER_LEADERBOARD_H
