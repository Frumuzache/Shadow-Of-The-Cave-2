#include "../header/Player.h"
#include <cmath>
#include <iostream>
#include <SFML/System/Angle.hpp>
#include "../header/GameException.h"

Player::Player()
    : mRangedWeapon(),
      mMeleeWeapon(),
      // Initialize with dummy values, actual setup happens in loadThrowableWeapon
      mThrowableWeapon("Frag Grenade", 0.f, 0.f, 0.f, 0.f, 0.f, mGrenadeTexture)
{
    mMovementSpeed = 150.f;

    if (!mTexture.loadFromFile("../assets/player.png"))
        throw AssetLoadException("Player Texture", "../assets/player.png");

    std::cout << "Player texture size: " << mTexture.getSize().x
              << " x " << mTexture.getSize().y << std::endl;

    mSprite.setTexture(mTexture, true);
    mSprite.setPosition({400.f, 300.f});

    // --- Load Inventory ---
    loadRangedWeapon();
    loadMeleeWeapon();
    loadThrowableWeapon();
}

void Player::loadRangedWeapon() {
    // Delegate to the class
    mRangedWeapon.load();
}

void Player::loadMeleeWeapon() {
    // Delegate to the class
    mMeleeWeapon.load();
}

void Player::loadThrowableWeapon() {
    // Delegate to the class, passing the texture storage that Player owns
    mThrowableWeapon.load(mGrenadeTexture);
}

void Player::update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) {
    updateMovement(deltaTime, mapBounds);
    updateRotation(window);
    updateHealth(deltaTime);
}

void Player::updateRotation(const sf::RenderWindow& window) {
    sf::Vector2i mouseScreenPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mouseScreenPos);

    sf::Vector2f playerPos = mSprite.getPosition();
    sf::Vector2u size = mTexture.getSize();
    sf::Vector2f center = {
        playerPos.x + static_cast<float>(size.x) / 2.f,
        playerPos.y + static_cast<float>(size.y) / 2.f
    };

    sf::Vector2f diff = mouseWorldPos - center;
    sf::Angle angle = sf::radians(std::atan2(diff.y, diff.x));

    mRangedWeapon.update(center, angle);
    mMeleeWeapon.update(center, angle);
}

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

    sf::Vector2f pos = mSprite.getPosition();
    sf::Vector2u size = mTexture.getSize();

    if (pos.x < 0.f) pos.x = 0.f;
    if (pos.y < 0.f) pos.y = 0.f;
    if (pos.x + static_cast<float>(size.x) > mapBounds.x) pos.x = mapBounds.x - static_cast<float>(size.x);
    if (pos.y + static_cast<float>(size.y) > mapBounds.y) pos.y = mapBounds.y - static_cast<float>(size.y);

    mSprite.setPosition(pos);
}

void Player::render(sf::RenderWindow& window) const {
    Entity::render(window);
    mRangedWeapon.render(window);
}

Player& Player::getInstance() {
    static Player instance;
    return instance;
}

RangedWeapon& Player::getRangedWeapon() { return mRangedWeapon; }
MeleeWeapon& Player::getMeleeWeapon() { return mMeleeWeapon; }
ThrowableWeapon& Player::getThrowableWeapon() { return mThrowableWeapon; }
Weapon& Player::getActiveWeapon() { return mRangedWeapon; }
void Player::updateHealth(sf::Time deltaTime) { (void)deltaTime; }

void Player::takeDamage(float damage) {
    if (currentHealth > 0) {
        currentHealth -= damage;
        if (currentHealth < 0) currentHealth = 0;
        std::cout << "Player took damage! Current HP: " << currentHealth << "\n";
    }
}

bool Player::isDead() const {
    return currentHealth <= 0;
}

sf::Vector2f Player::getPlayerPosition() const {
    return mSprite.getPosition();
}

sf::Vector2u Player::getTextureSize() const {
    return mTexture.getSize();
}

// sf::Angle Player::getRotation() const {
//     return mRangedWeapon.getRotation();
// }
