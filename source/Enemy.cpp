#include "../header/Enemy.h"
#include "../header/Player.h" // For movement logic
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <ostream> // For std::ostream

Enemy::Enemy()
    :Entity(80.f, 50.f), // Call base constructor (Speed, Health)
    mDirection(0.f, 0.f),
    initialPosition(100.f, 100.f)

{
    if (!mTexture.loadFromFile("../assets/enemy.png"))
        throw std::runtime_error("Failed to load enemy texture");

    std::cout << "Enemy texture loaded." << std::endl;
    mSprite.setTexture(mTexture, true);
    mSprite.setPosition(initialPosition);
}

// Parameterized constructor
Enemy::Enemy(sf::Vector2f startPosition, float speed, float health)
    :Entity(speed, health),
    mDirection(0.f, 0.f),
    initialPosition(startPosition)
{

    if (!mTexture.loadFromFile("../assets/enemy.png"))
        throw std::runtime_error("Failed to load enemy texture");

    std::cout << "Enemy texture loaded." << std::endl;
    mSprite.setTexture(mTexture, true);
    mSprite.setPosition(startPosition);
}

void Enemy::takeDamage(float damageAmount) {
    currentHealth -= damageAmount;
}

// ... (getCurrentHealth is now inherited from Entity) ...

void Enemy::death() {
    std::cout << "Enemy has died." << std::endl;
}

void Enemy::updateMovementEnemy(sf::Time deltaTime, sf::Window const& window) {
    // (Movement logic unchanged)
    const Player& player = Player::getInstance();
    sf::Vector2f playerPosition = player.getPlayerPosition();
    sf::Vector2u playerSpriteSize = player.getTextureSize();

    sf::Vector2f currentEnemyPos = mSprite.getPosition();
    sf::Vector2u enemySpriteSize = mTexture.getSize();
    sf::Vector2u windowSize = window.getSize();

    sf::Vector2f enemyCenter(
        currentEnemyPos.x + static_cast<float>(enemySpriteSize.x) * 0.5f,
        currentEnemyPos.y + static_cast<float>(enemySpriteSize.y) * 0.5f
    );

    sf::Vector2f playerCenter(
         playerPosition.x + static_cast<float>(playerSpriteSize.x) * 0.5f,
         playerPosition.y + static_cast<float>(playerSpriteSize.y) * 0.5f
     );

    sf::Vector2f direction = playerCenter - enemyCenter;
    sf::Vector2f movement(0.f, 0.f);

    constexpr float deadZoneRadius = 5.0f;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > deadZoneRadius) {
        movement = direction / distance;
        movement.x *= mMovementSpeed * deltaTime.asSeconds();
        movement.y *= mMovementSpeed * deltaTime.asSeconds();
    }

    sf::Vector2f newPosition = currentEnemyPos + movement;

    if (newPosition.x < 0.f) newPosition.x = 0.f;
    if (newPosition.y < 0.f) newPosition.y = 0.f;

    // Cast the unsigned ints to floats for the comparison
    if (newPosition.x + static_cast<float>(enemySpriteSize.x) > static_cast<float>(windowSize.x))
        // Cast BOTH before subtracting to avoid unsigned int math errors (like underflow)
            newPosition.x = static_cast<float>(windowSize.x) - static_cast<float>(enemySpriteSize.x);

    if (newPosition.y + static_cast<float>(enemySpriteSize.y) > static_cast<float>(windowSize.y))
        newPosition.y = static_cast<float>(windowSize.y) - static_cast<float>(enemySpriteSize.y);

    mSprite.setPosition(newPosition);
}

void Enemy::update(sf::Time deltaTime, const sf::Window& window) {
    updateMovementEnemy(deltaTime, window);

}

// --- operator<< (Composition of calls) ---
std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
    // 1. Call Entity::operator<<
    os << "--- ENEMY ---\n"
       << static_cast<const Entity&>(enemy);
    return os;
}