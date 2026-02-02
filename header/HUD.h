#pragma once
#include "Player.h"
#include "Inventory.h"
#include <SFML/Graphics.hpp>

class HUD {
public:
    explicit HUD(std::string fontPath);
    ~HUD();  // Destructor to clean up dynamically allocated Text objects

    // Delete copy constructor and assignment operator (rule of five)
    HUD(const HUD&) = delete;
    HUD& operator=(const HUD&) = delete;

    void update(const Player& player, const sf::RenderWindow& window, sf::Time deltaTime, int coins, const Inventory& inventory);

    void render(sf::RenderWindow& window) const;
    void renderGameOver(sf::RenderWindow& window);


    friend std::ostream& operator<<(std::ostream& os, const HUD& hud);

private:
    std::string mFontPath;
    sf::Font mFont;

    sf::Text* mCoinsText = nullptr;         // Display coins
    sf::Text* mInventoryText = nullptr;     // Display inventory items
    sf::Text* mShopText = nullptr;          // Display shop keybinds/costs

    sf::Text* mPlayerHealthText = nullptr;
    sf::Text* mTimerText = nullptr;
    sf::Text* mGameOverText = nullptr;

    sf::Clock mLevelClock;
    sf::RectangleShape mDamageOverlay;
    float mFlashAlpha;
    float mPreviousHealth;
};