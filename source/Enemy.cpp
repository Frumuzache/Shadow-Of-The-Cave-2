#include "../header/Enemy.h"
#include "../header/Player.h" // Required to access Player singleton
#include <stdexcept>
#include <iostream>
#include <cmath>

// Default Constructor
Enemy::Enemy()
    : Entity(200.f, 50.f),
      mDirection(0.f, 0.f),
      initialPosition(100.f, 100.f),
      // Initialize private weapon: Name, Damage, Cooldown, Range
      mWeapon("Zombie Claws", 10.f, 1.0f, 100.f)
{
    loadAssets();
    mSprite.setPosition(initialPosition);
}

// Parameterized Constructor
Enemy::Enemy(const sf::Vector2f startPosition, const float speed, const float health)
    : Entity(speed, health),
      mDirection(0.f, 0.f),
      initialPosition(startPosition),
      // Initialize private weapon
      mWeapon("Zombie Claws", 10.f, 1.0f, 100.f)
{
    loadAssets();
    mSprite.setPosition(startPosition);
}

void Enemy::loadAssets() {
    if (!mTexture.loadFromFile("../assets/enemy.png"))
        throw std::runtime_error("Failed to load enemy texture");
    mSprite.setTexture(mTexture, true);
}

// Main Update Loop
void Enemy::update(const sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) {
    // 1. Handle Movement
    updateMovementEnemy(deltaTime, mapBounds);

    // 2. Handle Combat (Try to attack player)
    tryAttack();

    // Suppress unused variable warning if window isn't used for logic
    (void)window;
}

// Combat Logic
void Enemy::tryAttack() {
    // Access the Player Singleton
    Player& player = Player::getInstance();

    // Optimization: Don't attack if player is already dead
    if (player.isDead()) return;

    // 1. Calculate Centers for accurate distance
    sf::Vector2f playerPos = player.getPlayerPosition();
    sf::Vector2u pSize = player.getTextureSize();
    sf::Vector2f playerCenter = playerPos + sf::Vector2f(static_cast<float>(pSize.x) * 0.5f, static_cast<float>(pSize.y) * 0.5f);

    sf::Vector2f enemyPos = mSprite.getPosition(); // Inherited from Entity
    sf::Vector2u eSize = mTexture.getSize();       // Inherited from Entity
    sf::Vector2f enemyCenter = enemyPos + sf::Vector2f(static_cast<float>(eSize.x) * 0.5f, static_cast<float>(eSize.y) * 0.5f);

    // 2. Calculate Distance
    sf::Vector2f diff = playerCenter - enemyCenter;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    // 3. Check Range (using private weapon stats)
    if (distance <= mWeapon.getRange()) {
        // 4. Check Cooldown
        if (mAttackClock.getElapsedTime().asSeconds() >= mWeapon.getReloadTime()) {
            // Attack!
            player.takeDamage(mWeapon.getDamage());
            mAttackClock.restart(); // Reset cooldown
        }
    }
}

// Movement Logic
void Enemy::updateMovementEnemy(sf::Time deltaTime, const sf::Vector2f& mapBounds) {
    const Player& player = Player::getInstance();

    sf::Vector2f playerPos = player.getPlayerPosition();
    sf::Vector2u pSize = player.getTextureSize();
    sf::Vector2f playerCenter = playerPos + sf::Vector2f(static_cast<float>(pSize.x) * 0.5f, static_cast<float>(pSize.y) * 0.5f);

    sf::Vector2f enemyPos = mSprite.getPosition();
    sf::Vector2u eSize = mTexture.getSize();
    sf::Vector2f enemyCenter = enemyPos + sf::Vector2f(static_cast<float>(eSize.x) * 0.5f, static_cast<float>(eSize.y) * 0.5f);

    sf::Vector2f direction = playerCenter - enemyCenter;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Move only if not touching player (keep a small buffer)
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

void Enemy::takeDamage(float damageAmount) {
    // Modify protected variable 'currentHealth' from Entity
    currentHealth -= damageAmount;
}

void Enemy::death() {
    std::cout << "Enemy has died.\n";
}

void Enemy::render(sf::RenderWindow &window) const {
    window.draw(mSprite);
}

std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
    os << "--- ENEMY ---\n" << static_cast<const Entity&>(enemy);
    return os;
}