#include "../header/Enemy.h"
#include "../header/Player.h"
#include <stdexcept>
#include <iostream>
#include <cmath>

Enemy::Enemy() : Entity(80.f, 50.f), mDirection(0.f, 0.f), initialPosition(100.f, 100.f) {
    loadAssets();
    mSprite.setPosition(initialPosition);
}

Enemy::Enemy(sf::Vector2f startPosition, float speed, float health)
    : Entity(speed, health), mDirection(0.f, 0.f), initialPosition(startPosition) {
    loadAssets();
    mSprite.setPosition(startPosition);
}

// Helper to keep constructors clean
void Enemy::loadAssets() {
    if (!mTexture.loadFromFile("../assets/enemy.png"))
        throw std::runtime_error("Failed to load enemy texture");
    mSprite.setTexture(mTexture, true);
    std::cout << "Enemy texture loaded.\n";
}

void Enemy::update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) {
    updateMovementEnemy(deltaTime, mapBounds);
    /// solve tempory issue of unsused parameter
    (void)window;
}

void Enemy::updateMovementEnemy(sf::Time deltaTime, const sf::Vector2f& mapBounds) {
    const Player& player = Player::getInstance();

    // Calculate Centers
    sf::Vector2f playerPos = player.getPlayerPosition();
    sf::Vector2u pSize = player.getTextureSize();
    sf::Vector2f playerCenter = playerPos + sf::Vector2f(static_cast<float>(pSize.x) * 0.5f, static_cast<float>(pSize.y) * 0.5f);

    sf::Vector2f enemyPos = mSprite.getPosition();
    sf::Vector2u eSize = mTexture.getSize();
    sf::Vector2f enemyCenter = enemyPos + sf::Vector2f(static_cast<float>(eSize.x) * 0.5f, static_cast<float>(eSize.y) * 0.5f);

    // Calculate Direction
    sf::Vector2f direction = playerCenter - enemyCenter;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Move only if not touching player
    sf::Vector2f movement(0.f, 0.f);
    if (distance > 5.0f) {
        movement = (direction / distance) * mMovementSpeed * deltaTime.asSeconds();
    }

    sf::Vector2f newPos = enemyPos + movement;

    // Map Boundaries Check
    if (newPos.x < 0.f) newPos.x = 0.f;
    if (newPos.y < 0.f) newPos.y = 0.f;
    if (newPos.x + static_cast<float>(eSize.x) > mapBounds.x) newPos.x = mapBounds.x - static_cast<float>(eSize.x);
    if (newPos.y + static_cast<float>(eSize.y) > mapBounds.y) newPos.y = mapBounds.y - static_cast<float>(eSize.y);

    mSprite.setPosition(newPos);
}

void Enemy::takeDamage(float damageAmount) { currentHealth -= damageAmount; }
void Enemy::death() { std::cout << "Enemy has died.\n"; }
void Enemy::render(sf::RenderWindow &window) const { window.draw(mSprite); }

std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
    os << "--- ENEMY ---\n" << static_cast<const Entity&>(enemy);
    return os;
}