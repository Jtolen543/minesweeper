//
// Created by jtole on 7/24/2024.
//

#ifndef MINESWEEPER_TILE_H
#define MINESWEEPER_TILE_H
#include <SFML/Graphics.hpp>
#include "texturemanager.h"
#include <vector>
class Tile {
private:
    sf::Texture under;
    sf::Sprite sprite;
    sf::Sprite flag;
    sf::Sprite number;
    TextureManager textures;
    bool flagged = false;
    bool mine = false;
    bool hidden = true;
    bool no_neighbors = false;
    std::vector<Tile*> adjacent;
public:
    Tile(int x, int y) {
        x *= 32, y *=32;
        under = textures.tile_revealed;
        sf::Vector2f position((float)x, (float)y);
        sprite.setTexture(textures.tile_hidden);
        sprite.setPosition(position);
        flag.setTexture(textures.flag);
    }
    void flagTile() {
        if (hidden) {
            flag.setPosition(sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y));
            flagged = true;
        }
    }
    void clearFlag() {
        if (flagged) {
            flagged = false;
        }
    }
    bool isFlagged() const {
        return flagged;
    }
    sf::Sprite getSprite() {
        return sprite;
    }
    void Unreveal() {
        sprite.setTexture(under);
        clearFlag();
        hidden = false;
        if (!no_neighbors || isMine()) {
            return;
        }
        else {
            for (auto& tile: adjacent) {
                if (tile->isHidden() && !tile->isFlagged())
                    tile->Unreveal();
            }
        }
    }
    bool isHidden() {
        return hidden;
    }
    sf::Sprite getFlag() {
        return flag;
    }
    bool setMine() {
        if (!mine) {
            under = textures.mine;
            mine = true;
            return true;
        } else {
            return false;
        }
    }
    bool isMine() const {
        return mine;
    }
    void setAdjacentMines(std::vector<Tile*>& variable) {
        adjacent = variable;
        sf::Vector2f vector = sprite.getPosition();
        int count = 0;
        for (auto& tile: adjacent) {
            if (tile->isMine()) {
                count++;
            }
        }
        if (count == 0)
            no_neighbors = true;
        else if (isMine())
            return;
        else if (count == 1) {
            number.setTexture(textures.num_1);
            number.setPosition(vector);
        }
        else if (count == 2) {
            number.setTexture(textures.num_2);
            number.setPosition(vector);
        }
        else if (count == 3) {
            number.setTexture(textures.num_3);
            number.setPosition(vector);
        }
        else if (count == 4) {
            number.setTexture(textures.num_4);
            number.setPosition(vector);
        }
        else if (count == 5) {
            number.setTexture(textures.num_5);
            number.setPosition(vector);
        }
        else if (count == 6) {
            number.setTexture(textures.num_6);
            number.setPosition(vector);
        }
        else if (count == 7) {
            number.setTexture(textures.num_7);
            number.setPosition(vector);
        }
        else if (count == 8) {
            number.setTexture(textures.num_8);
            number.setPosition(vector);
        }
    }
    sf::Sprite getNumber() {
        return number;
    }
    sf::Sprite pauseTile() {
        sf::Vector2f vector;
        vector = sprite.getPosition();
        sf::Sprite dummy(textures.tile_revealed);
        dummy.setPosition(vector);
        return dummy;
    }

};
#endif //MINESWEEPER_TILE_H
