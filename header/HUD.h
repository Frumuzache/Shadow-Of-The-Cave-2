#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>

class HUD {
public:
    explicit HUD(std::string fontPath);

    // CHANGED: Added sf::Time deltaTime
    void update(const Player& player, const sf::RenderWindow& window, sf::Time deltaTime);

    void render(sf::RenderWindow& window) const;
    void renderGameOver(sf::RenderWindow& window);

    friend std::ostream& operator<<(std::ostream& os, const HUD& hud);

private:
    std::string mFontPath;
    sf::Font mFont;

    // DECLARATION ORDER MATTERS FOR CONSTRUCTOR
    sf::Text mPlayerHealthText;
    sf::Text mTimerText;
    sf::Text mGameOverText;

    sf::Clock mLevelClock;
    sf::RectangleShape mDamageOverlay;
    float mFlashAlpha;
    float mPreviousHealth;
};