#include "../header/Player.h"
#include <cmath>
#include <iostream>
#include <SFML/System/Angle.hpp>

Player::Player() {
    mMovementSpeed = 150.f;

    if (!mTexture.loadFromFile("../assets/player.png"))
        throw std::runtime_error("Failed to load player texture");

    std::cout << "Player texture size: " << mTexture.getSize().x
              << " x " << mTexture.getSize().y << std::endl;

    mSprite.setTexture(mTexture, true);
    mSprite.setPosition({400.f, 300.f});
}

// Main Update Loop
void Player::update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) {
    updateMovement(deltaTime, mapBounds);
    updateRotation(window);
    updateHealth(deltaTime);
}

// Logic: Rotation (Aiming at Mouse)
void Player::updateRotation(const sf::RenderWindow& window) {
    // 1. Get Mouse Position relative to the View
    sf::Vector2i mouseScreenPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseScreenPos);

    // 2. Calculate Center of Player (Hand position)
    sf::Vector2f playerPos = mSprite.getPosition();
    sf::Vector2u size = mTexture.getSize();
    sf::Vector2f center = {
        playerPos.x + static_cast<float>(size.x) / 2.f,
        playerPos.y + static_cast<float>(size.y) / 2.f
    };

    // 3. Calculate Angle
    sf::Vector2f diff = mouseWorldPos - center;
    sf::Angle angle = sf::radians(std::atan2(diff.y, diff.x));

    // 4. Update Weapon (Pass angle to weapon, don't rotate player body)
    mWeapon.update(center, angle);
}

// Logic: Movement (WASD + Borders)
void Player::updateMovement(sf::Time deltaTime, sf::Vector2f mapBounds) {
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += 1.f;

    if (movement.x != 0.f || movement.y != 0.f) {
        float len = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement.x = (movement.x / len) * mMovementSpeed * deltaTime.asSeconds();
        movement.y = (movement.y / len) * mMovementSpeed * deltaTime.asSeconds();
    }

    mSprite.move(movement);

    // Border Checks
    sf::Vector2f pos = mSprite.getPosition();
    sf::Vector2u size = mTexture.getSize();

    if (pos.x < 0.f) pos.x = 0.f;
    if (pos.y < 0.f) pos.y = 0.f;
    if (pos.x + static_cast<float>(size.x) > mapBounds.x) pos.x = mapBounds.x - static_cast<float>(size.x);
    if (pos.y + static_cast<float>(size.y) > mapBounds.y) pos.y = mapBounds.y - static_cast<float>(size.y);

    mSprite.setPosition(pos);
}

// Render
void Player::render(sf::RenderWindow& window) const {
    Entity::render(window); // Draw Body
    mWeapon.render(window); // Draw Weapon
}

// Getters & Singleton
Player& Player::getInstance() {
    static Player instance;
    return instance;
}

Weapon& Player::getWeapon() { return mWeapon; }
sf::Vector2f Player::getPlayerPosition() const { return mSprite.getPosition(); }
sf::Vector2u Player::getTextureSize() const { return mTexture.getSize(); }
sf::Angle Player::getRotation() const { return mSprite.getRotation(); }

void Player::updateHealth(sf::Time deltaTime) { (void)deltaTime; }