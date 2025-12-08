#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>

class HUD {
public:
    explicit HUD(std::string fontPath);

    void update(const Player& player, const sf::RenderWindow& window, sf::Time deltaTime);

    void render(sf::RenderWindow& window) const;
    void renderGameOver(sf::RenderWindow& window);

    void update(const Player& player, const sf::RenderWindow& window, sf::Time deltaTime, int kills);

    friend std::ostream& operator<<(std::ostream& os, const HUD& hud);

private:
    std::string mFontPath;
    sf::Font mFont;

    sf::Text mKillCountText;

    sf::Text mPlayerHealthText;
    sf::Text mTimerText;
    sf::Text mGameOverText;

    sf::Clock mLevelClock;
    sf::RectangleShape mDamageOverlay;
    float mFlashAlpha;
    float mPreviousHealth;
};