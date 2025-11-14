#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <ostream> // For std::ostream
#include "Enemy.h"
#include "Player.h"
#include "Level.h" // Include new class
#include "HUD.h"   // Include new class

class Game {
public:
    // Parameterized constructor
    Game(unsigned int width, unsigned int height, const std::string& title);

    void run();

    sf::Vector2u getWindowSize() const; // Made const

    // operator<< for display
    friend std::ostream& operator<<(std::ostream& os, const Game& game);

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    // Composition: Game "has-a" window, clock, level, hud, player, and enemies
    sf::RenderWindow mWindow;
    sf::Clock mClock;

    Level mLevel; // Replaces background texture/sprite
    HUD mHUD;     // New composed object

    Player& mPlayer; // Get the singleton instance
    std::vector<std::unique_ptr<Enemy>> mEnemies;
};

#endif //GAME_H