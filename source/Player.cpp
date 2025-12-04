#include "../header/Player.h"
#include <cmath>
#include <iostream>
// Note: Game.h is no longer needed here, but Player.h includes SFML
// via Entity.h, so Graphics.hpp is not needed either.

// Constructor
Player::Player()
{
    // Initialize the members we inherited from Entity
    mMovementSpeed = 150.f;

    if (!mTexture.loadFromFile("../assets/player.png"))
        throw std::runtime_error("Failed to load player texture");

    std::cout << "Player texture size: " << mTexture.getSize().x
            << " x " << mTexture.getSize().y << std::endl;

    mSprite.setTexture(mTexture, true); // mSprite is from Entity
    mSprite.setPosition({400.f, 300.f}); // Set initial position
}

// This is the main update function required by Entity
void Player::update(sf::Time deltaTime, const sf::Vector2f& mapBounds)
{
    // It just calls the specific player functions
    updateMovement(deltaTime, mapBounds);
    updateHealth(deltaTime);
}

Weapon& Player::getWeapon()
{
    return mWeapon;
}

sf::Vector2f Player::getPlayerPosition() const
{
    return mSprite.getPosition();
}

Player& Player::getInstance() {
    static Player instance; // The one and only Player object
    return instance;
}

sf::Vector2u Player::getTextureSize() const
{
    // Assuming your player's texture is also named mTexture
    return mTexture.getSize();
}


void Player::updateMovement(sf::Time deltaTime, sf::Vector2f mapBounds)
{
    sf::Vector2f movement(0.f, 0.f);

    // Check for real-time keyboard input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        movement.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        movement.x += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        movement.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        movement.y += 1.f;
    }

    if (movement.x != 0.f || movement.y != 0.f) {
        float len = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement.x = (movement.x / len) * mMovementSpeed * deltaTime.asSeconds();
        movement.y = (movement.y / len) * mMovementSpeed * deltaTime.asSeconds();
    }

    mSprite.move(movement);

    // Border collision (keep player within window bounds)
    sf::Vector2f position = mSprite.getPosition();
    sf::Vector2u spriteSize = mTexture.getSize();

    // Definim limita HARDCODED la 3000 (sau o primești ca parametru)
    // Trebuie să fie fix cât ai pus în Level.cpp!
    float mapLimitX = 3000.f;
    float mapLimitY = 3000.f;

    // Stânga și Sus (Nu trecem de 0)
    if (position.x < 0.f) position.x = 0.f;
    if (position.y < 0.f) position.y = 0.f;

    // Dreapta (Nu trecem de 3000)
    if (position.x + static_cast<float>(spriteSize.x) > mapLimitX)
        position.x = mapLimitX - static_cast<float>(spriteSize.x);

    // Jos (Nu trecem de 3000)
    if (position.y + static_cast<float>(spriteSize.y) > mapLimitY)
        position.y = mapLimitY - static_cast<float>(spriteSize.y);

    mSprite.setPosition(position);
}






void Player::updateHealth(sf::Time deltaTime) {
    // Placeholder for health update logic
   (void)deltaTime; // silence C4100: unreferenced parameter (warnings-as-errors in CI)
}