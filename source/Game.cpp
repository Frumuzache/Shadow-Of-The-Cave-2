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

      mLevel("../assets/background.png", sf::Vector2f(3000.f, 3000.f)),
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

    // const Weapon testWeapon = mPlayer.getWeapon(); // Tests Copy Constructor
    // const Weapon& testWeapon2 = testWeapon;
    // (void)testWeapon2;
    //
    // std::cout << "--- End of Test ---\n" << std::endl;

    mWorldSize = sf::Vector2f(3000.f, 3000.f);
    mView.setSize({static_cast<float>(width), static_cast<float>(height)});

    mView.setCenter({static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f});


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
                float attackRange = Player::getAttackRange();
                float attackDamage = Player::getAttackDamage();

                sf::Vector2f playerPosition = mPlayer.getPlayerPosition();
                sf::Vector2u playerSpriteSize = mPlayer.getTextureSize();

                sf::Vector2f enemyPosition = mEnemy->getPosition();
                sf::Vector2u enemySpriteSize = mEnemy->getSpriteSize();

                float distanceX = (playerPosition.x + static_cast<float>(playerSpriteSize.x) / 2.0f) - (enemyPosition.x + static_cast<float>(enemySpriteSize.x) / 2.0f);
                float distanceY = (playerPosition.y + static_cast<float>(playerSpriteSize.y) / 2.0f) - (enemyPosition.y + static_cast<float>(enemySpriteSize.y) / 2.0f);
                float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

                if (distance <= attackRange)
                    mEnemy->takeDamage(attackDamage);
            }
        }
    }
}

// Function to update the game state
void Game::update(sf::Time deltaTime) {
    mPlayer.update(deltaTime, mWorldSize); // Update player from member
    mHUD.update(mPlayer); // Update HUD with player's new state

    for (auto& enemy : mEnemies)
        enemy->update(deltaTime, mWorldSize);

    std::erase_if(mEnemies, [](const auto& enemy) {
        if (enemy->getCurrentHealth() <= 0) {
            enemy->death();
            return true;
        }
        return false;
    });


    resolveEnemyCollisions();


    // --- CAMERA LOGIC STARTS HERE ---
    sf::Vector2f targetPos = mPlayer.getPlayerPosition();
    sf::Vector2f viewSize = mView.getSize();

    // Calculate the center of the player sprite to center the camera accurately
    sf::Vector2u playerSize = mPlayer.getTextureSize();
    targetPos.x += static_cast<float>(playerSize.x) / 2.f;
    targetPos.y += static_cast<float>(playerSize.y) / 2.f;

    float halfWidth = viewSize.x / 2.0f;
    float halfHeight = viewSize.y / 2.0f;

    // Clamp X (Horizontal)
    if (targetPos.x < halfWidth) {
        targetPos.x = halfWidth;
    }
    else if (targetPos.x > mWorldSize.x - halfWidth) {
        targetPos.x = mWorldSize.x - halfWidth;
    }

    // Clamp Y (Vertical)
    if (targetPos.y < halfHeight) {
        targetPos.y = halfHeight;
    }
    else if (targetPos.y > mWorldSize.y - halfHeight) {
        targetPos.y = mWorldSize.y - halfHeight;
    }

    mView.setCenter(targetPos);
    // --- CAMERA LOGIC ENDS HERE ---


    mHUD.update(mPlayer);
}

// Function to draw everything
void Game::render() {
    mWindow.clear(sf::Color::Black);

    mWindow.setView(mView);


    // Draw composed objects
    mLevel.render(mWindow); // Draw the level
    mPlayer.render(mWindow); // Draw the player

    for (const auto& enemy : mEnemies) {
        enemy->render(mWindow);
    }

    mWindow.setView(mWindow.getDefaultView());
    mHUD.render(mWindow); // Draw the HUD on top
    mWindow.display();
}





void Game::resolveEnemyCollisions() const {
    // Loop through all unique pairs of enemies
    for (size_t i = 0; i < mEnemies.size(); ++i) {
        for (size_t j = i + 1; j < mEnemies.size(); ++j) {

            auto* enemyA = mEnemies[i].get();
            auto* enemyB = mEnemies[j].get();

            // 1. Calculate Centers
            sf::Vector2f posA = enemyA->getPosition();
            sf::Vector2u sizeA = enemyA->getSpriteSize();
            sf::Vector2f centerA = {
                posA.x + static_cast<float>(sizeA.x) / 2.f,
                posA.y + static_cast<float>(sizeA.y) / 2.f
            };

            sf::Vector2f posB = enemyB->getPosition();
            sf::Vector2u sizeB = enemyB->getSpriteSize();
            sf::Vector2f centerB = {
                posB.x + static_cast<float>(sizeB.x) / 2.f,
                posB.y + static_cast<float>(sizeB.y) / 2.f
            };

            // 2. Check Distance
            sf::Vector2f diff = centerA - centerB;
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

            // Define a minimum distance based on enemy size (approx. radius)
            // Using 80% of the width ensures they don't overlap too much but don't gap too wide
            float minDistance = (static_cast<float>(sizeA.x + sizeB.x)) * 0.4f;

            // 3. Resolve Collision (Push them apart)
            if (distance < minDistance) {
                // Calculate push vector
                sf::Vector2f pushVector;

                if (distance == 0.f) {
                    // If exact same position, pick random direction
                    pushVector = {1.f, 0.f};
                } else {
                    // Normalization: (diff / distance)
                    pushVector = diff / distance;
                }

                // How much to push (split between both enemies)
                float pushStrength = (minDistance - distance) / 2.f;

                // Move Enemy A away
                sf::Vector2f newPosA = posA + (pushVector * pushStrength);
                enemyA->setPosition(newPosA); // You might need to add setPosition to Entity if not there

                // Move Enemy B the opposite way
                sf::Vector2f newPosB = posB - (pushVector * pushStrength);
                enemyB->setPosition(newPosB);
            }
        }
    }
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