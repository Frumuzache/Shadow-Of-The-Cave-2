#include <SFML/Graphics.hpp>
#include "../header/Game.h"
#include "../header/Player.h"
#include <cmath>
#include <iostream> // For std::ostream
#include <ostream>  // For std::ostream

// Parameterized constructor
Game::Game(unsigned int width, unsigned int height, const std::string& title)
    : mWindow(sf::VideoMode({width, height}), title),
      mClock{},
      // --- Initialize composed objects ---
      mLevel("../assets/background.png"),
      mHUD("../assets/arial.ttf"), // <-- Assumes this font exists
      mPlayer(Player::getInstance()) // Get singleton instance
{
    mWindow.setFramerateLimit(120);

    // Use parameterized constructor for Enemies
    mEnemies.push_back(std::make_unique<Enemy>(sf::Vector2f(300.f, 100.f), 80.f, 50.f));
    mEnemies.push_back(std::make_unique<Enemy>(sf::Vector2f(600.f, 400.f), 80.f, 50.f));
    mEnemies.push_back(std::make_unique<Enemy>(sf::Vector2f(900.f, 700.f), 80.f, 50.f));

    // --- Example of operator<< ---
    std::cout << "Game created. Initial state:\n" << *this << std::endl;

    // --- Example of Rule of Three ---
    std::cout << "\n--- Testing Rule of Three for Weapon ---\n";
    Weapon testWeapon = mPlayer.getWeapon(); // Tests Copy Constructor
    Weapon testWeapon2;
    testWeapon2 = testWeapon; // Tests Copy Assignment
    std::cout << "--- End of Test ---\n" << std::endl;
    // testWeapon and testWeapon2 will be destroyed here, testing Destructor
}

// The main run function
void Game::run() {
    while (mWindow.isOpen()) {
        sf::Time deltaTime = mClock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = mWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                mWindow.close();
        }

        // Using isKeyPressed for continuous actions like attacking is fine
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            for (const auto& enemyPtr : mEnemies) {
                Enemy* mEnemy = enemyPtr.get();
                if (!mEnemy) continue;

                // Get attack stats from the Player object's weapon
                float attackRange = mPlayer.getAttackRange();
                float attackDamage = mPlayer.getAttackDamage();

                sf::Vector2f playerPosition = mPlayer.getPlayerPosition();
                sf::Vector2u playerSpriteSize = mPlayer.getTextureSize();

                sf::Vector2f enemyPosition = mEnemy->getPosition();
                sf::Vector2u enemySpriteSize = mEnemy->getSpriteSize();

                float distanceX = (playerPosition.x + playerSpriteSize.x / 2) - (enemyPosition.x + enemySpriteSize.x / 2);
                float distanceY = (playerPosition.y + playerSpriteSize.y / 2) - (enemyPosition.y + enemySpriteSize.y / 2);
                float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

                if (distance <= attackRange)
                    mEnemy->takeDamage(attackDamage);
            }
        }
    }
}

// Function to update the game state
void Game::update(sf::Time deltaTime) {
    mPlayer.update(deltaTime, mWindow); // Update player from member
    mHUD.update(mPlayer); // Update HUD with player's new state

    for (auto& enemy : mEnemies)
        enemy->update(deltaTime, mWindow);

    std::erase_if(mEnemies, [](const auto& enemy) {
        if (enemy->getCurrentHealth() <= 0) {
            enemy->death();
            return true;
        }
        return false;
    });
}

// Function to draw everything
void Game::render() {
    mWindow.clear(sf::Color::Black);

    // Draw composed objects
    mLevel.render(mWindow); // Draw the level
    mPlayer.render(mWindow); // Draw the player

    for (auto& enemy : mEnemies) {
        enemy->render(mWindow);
    }

    mHUD.render(mWindow); // Draw the HUD on top
    mWindow.display();
}

sf::Vector2u Game::getWindowSize() const {
    return mWindow.getSize();
}

// --- operator<< (Composition of calls) ---
std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "====== GAME STATE ======\n";
    os << "* " << game.mLevel << "\n"; // Calls Level::operator<<
    os << "* " << game.mHUD << "\n";   // Calls HUD::operator<<
    os << "* " << game.mPlayer << "\n"; // Calls Player::operator<<
    os << "* Enemies: " << game.mEnemies.size() << "\n";
    for (const auto& enemy : game.mEnemies) {
        os << "  " << *enemy << "\n"; // Calls Enemy::operator<<
    }
    os << "========================\n";
    return os;
}