#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "gameboard.h"
#include "texturemanager.h"
#include "leaderboard.h"
using namespace std;

void setText(sf::Text &text, float x, float y) { // Used to center the text for the display messages
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                     textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
    }

int main() {
    ifstream file("../files/config.cfg");
    if (!file) {
        cout << "Unable to open file!" << endl;
        return 0;
    }
    string line;
    (getline(file,line));
    int num_cols = stoi(line);
    (getline(file,line));
    int num_rows = stoi(line);
    (getline(file,line));
    int num_mines = stoi(line);
    //// CODE FOR WELCOME WINDOW SCREEN !!!!
    sf::RenderWindow welcomeWindow(sf::VideoMode(num_cols * 32, num_rows * 32 + 100),
                                   "Minesweeper");
    sf::Font font;
    font.loadFromFile("../files/font.ttf"); // Loads the font for the welcome screen
    float x = (float)num_cols * 16;
    float y1 = ((float)num_rows * 32 + 100)/2 - 150;
    float y2 = ((float)num_rows * 32 + 100)/2 - 75;
    float y3 = ((float)num_rows * 32 + 100)/2 - 45;
    sf::Text welcome_message("WELCOME TO MINESWEEPER!", font, 30);
    welcome_message.setStyle(sf::Text::Bold);
    welcome_message.setStyle(sf::Text::Underlined);
    welcome_message.setFillColor(sf::Color::White);
    sf::Text input_name("Enter your name:", font, 20);
    input_name.setStyle(sf::Text::Bold);
    input_name.setFillColor(sf::Color::White);
    sf::Text typed_name("", font, 18);
    sf::String player_input = "";
    typed_name.setStyle(sf::Text::Bold);
    typed_name.setFillColor(sf::Color::Yellow);
    setText(welcome_message,x, y1);
    setText(input_name, x, y2);
    setText(typed_name, x, y3);
    sf::RectangleShape cursor(sf::Vector2f(3, 20));
    cursor.setFillColor(sf::Color::White);
    cursor.setPosition(x, y3 - 9);
    bool show_cursor = true;
    sf::Clock clock;

    while(welcomeWindow.isOpen()) {
        sf::Event event; //NOLINT
        if (clock.getElapsedTime() >= sf::milliseconds(500)) {
            clock.restart();
            show_cursor = !show_cursor;
        }
        while(welcomeWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                welcomeWindow.close();
                return 0;
            }
            if (event.type == sf::Event::TextEntered && isalpha(event.text.unicode)) { //NOLINT
                if (player_input.getSize() < 10) {
                    player_input += event.text.unicode;
                    if (player_input[0] >= 'a' && player_input[0] <= 'z') {
                        player_input[0] = toupper(player_input[0]); //NOLINT
                    }
                    typed_name.setString(player_input);
                    setText(typed_name, x, y3);
                    cursor.setPosition(typed_name.getGlobalBounds().left + typed_name.getGlobalBounds().width+3, y3 - 9);
                    clock.restart();
                    show_cursor = true;
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace) {
                player_input = player_input.substring(0, player_input.getSize() - 1);
                typed_name.setString(player_input);
                setText(typed_name, x, y3);
                cursor.setPosition(typed_name.getGlobalBounds().left + typed_name.getGlobalBounds().width+3, y3 - 9);
                clock.restart();
                show_cursor = true;
            }
            else if (event.type == sf::Event::KeyPressed &&event.key.code == sf::Keyboard::Enter && player_input.getSize() > 0) {
                welcomeWindow.close();
            }
        }
        welcomeWindow.clear(sf::Color(0,0,255));
        if (show_cursor) {
            welcomeWindow.draw(cursor);
        }
        welcomeWindow.draw(welcome_message);
        welcomeWindow.draw(input_name);
        welcomeWindow.draw(typed_name);
        welcomeWindow.display();
    }
    sf::RenderWindow gameWindow(sf::VideoMode(num_cols * 32, num_rows * 32 + 100),
                                   "Minesweeper");
    GameBoard gameboard(num_cols, num_rows, num_mines);
    gameboard.generateBoard();
    int timer = 0; // Need to first see if game is paused, if paused, continue restarting the clock
    int save_time = 0;
    int formatted_time = 0;
    bool debug_mode = false;
    bool game_paused = false;
    bool game_over = false;
    bool game_lost = false;
    bool leader_paused = false;
    while (gameWindow.isOpen()) {
        sf::Event event; //NOLINT
        if (game_paused) {
            clock.restart();
        }
        else {
            timer = save_time + clock.getElapsedTime().asSeconds(); //NOLINT
            formatted_time = (timer / 60) * 100 + (timer % 60);
        }
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.y <= num_rows * 32 && !game_paused) {
                if (event.mouseButton.button == sf::Mouse::Left && !game_over) {
                    gameboard.leftClickedTile(event.mouseButton.x, event.mouseButton.y);
                    if (gameboard.checkGameStatus() == "Continue)") {
                        game_over = false;
                    }
                    else if (gameboard.checkGameStatus() == "Win") {
                        gameboard.switchFaceMode(true);
                        game_over = true;
                        game_paused = true;
                        gameboard.switchPlayMode(game_paused, gameWindow);
                        gameboard.updateLeaderBoard(player_input, formatted_time);
                    }
                    else if (gameboard.checkGameStatus() == "Lose") {
                        gameboard.switchFaceMode(false);
                        game_over = true;
                        game_paused = true;
                        game_lost = true;
                        debug_mode = false;
                        gameboard.switchPlayMode(game_paused, gameWindow);
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right && !game_over) {
                    gameboard.rightClickedTile(event.mouseButton.x, event.mouseButton.y);
                    if (gameboard.checkGameStatus() == "Win") {
                        gameboard.switchFaceMode(true);
                        game_over = true;
                        game_paused = true;
                        debug_mode = false;
                        gameboard.switchPlayMode(game_paused, gameWindow);
                        gameboard.updateLeaderBoard(player_input, formatted_time);
                    }
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (gameboard.isDebugButton(event.mouseButton.x, event.mouseButton.y) && !game_over) {
                        debug_mode = !debug_mode;
                    }
                    else if (gameboard.isPausePlayButton(event.mouseButton.x, event.mouseButton.y)  && !game_over) {
                        if (!game_paused) {
                            save_time = timer;
                        }
                        game_paused = !game_paused;
                        gameboard.switchPlayMode(game_paused, gameWindow);
                    }
                    else if (gameboard.isFaceButton(event.mouseButton.x, event.mouseButton.y)) {
                        gameboard.Restart(num_cols, num_rows, num_mines);
                        timer = 0;
                        save_time = 0;
                        formatted_time = 0;
                        clock.restart();
                        gameboard.switchPlayMode(false, gameWindow);
                        game_paused = false;
                        debug_mode = false;
                        game_over = false;
                        game_lost = false;
                    }
                    else if (gameboard.isLeaderboardButton(event.mouseButton.x, event.mouseButton.y)) {
                        if (!game_paused) {
                            save_time = timer;
                            game_paused = true;
                            gameboard.switchPlayMode(game_paused, gameWindow);
                            gameWindow.display();
                        }
                        else leader_paused = true;
                        sf::RenderWindow leaderBoard(sf::VideoMode(num_cols * 32,
                                                                    num_rows * 32 + 100),"Minesweeper");
                        LeaderBoard leader_board(leaderBoard,(float)leaderBoard.getPosition().x,
                                                 (float)leaderBoard.getPosition().y, font);
                        while (leaderBoard.isOpen()) {
                            sf::Event new_event;
                            while (leaderBoard.pollEvent(new_event)) {
                                clock.restart();
                                if (new_event.type == sf::Event::Closed) {
                                    leaderBoard.close();
                                }
                                leaderBoard.clear(sf::Color(0,0,255));
                                leader_board.writeText(leaderBoard);
                                leaderBoard.display();
                            }
                        }
                        while (gameWindow.pollEvent(event)) {
                        }
                        if (!game_over) {
                            if (!leader_paused) {
                                game_paused = false;
                                gameboard.switchPlayMode(game_paused, gameWindow);
                                gameWindow.display();
                            }
                            else leader_paused = false;
                            }
                    }
                }
            }
        }
        gameWindow.clear(sf::Color(255,255,255));
        gameboard.drawToWindow(gameWindow,formatted_time);
        if (game_over && !game_lost) {
            gameWindow.display();
           continue;
        }
        else if (game_lost) {
            gameboard.displaySolution(gameWindow);
        }
        else if (game_paused) {
            gameboard.displayPausedBoard(gameWindow);
            gameWindow.display();
            continue;
        }
        else if (debug_mode) {
            gameboard.Debug(gameWindow);
        }
        gameWindow.display();
    }
    return 0;
}
