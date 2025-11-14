#ifndef HUD_H
#define HUD_H
#include <string>
#include <SFML/Graphics.hpp>
#include "Player.h"

class HUD {
public:
    // Parameterized constructor
    explicit HUD(std::string fontPath);

    void update(const Player& player);
    void render(sf::RenderWindow& window) const;

    // operator<< for display
    friend std::ostream& operator<<(std::ostream& os, const HUD& hud);

private:
    std::string mFontPath;
    sf::Font mFont;
    sf::Text mPlayerHealthText;
};

#endif //HUD_H