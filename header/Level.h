#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <string>

class Level {
public:
    explicit Level(std::string texturePath, sf::Vector2f mapBoundries);

    void render(sf::RenderWindow& window) const;

    friend std::ostream& operator<<(std::ostream& os, const Level& level);

private:
    std::string mTexturePath;
    sf::Texture mBackgroundTexture;
    sf::Sprite mBackgroundSprite;
};

#endif //LEVEL_H