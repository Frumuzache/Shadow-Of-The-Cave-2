#include "../header/Entity.h"
#include <ostream>

Entity::Entity() : Entity(100.f, 100.f) {}

Entity::Entity(float speed, float health)
    : mTexture{}, mSprite(mTexture),
      mMovementSpeed(speed), maxHealth(health), currentHealth(health)
{}

// Getters
sf::FloatRect Entity::getGlobalBounds() const { return mSprite.getGlobalBounds(); }
sf::Vector2f Entity::getPosition() const { return mSprite.getPosition(); }
sf::Vector2u Entity::getSpriteSize() const { return mTexture.getSize(); }
float Entity::getCurrentHealth() const { return currentHealth; }

void Entity::takeDamage(float damage) {
    currentHealth -= damage;
    if (currentHealth < 0.f) currentHealth = 0.f;
}

void Entity::render(sf::RenderWindow& window) const {
    window.draw(mSprite);
}

sf::Vector2f Entity::setPosition(const sf::Vector2f &position) {
    mSprite.setPosition(position);
    return mSprite.getPosition();
}

std::ostream& operator<<(std::ostream& os, const Entity& entity) {
    os << "Entity(HP:" << entity.currentHealth << "/" << entity.maxHealth
       << ", Pos:{" << entity.getPosition().x << "," << entity.getPosition().y << "})";
    return os;
}