#include "../header/Entity.h"
#include <ostream> // For std::ostream

Entity::Entity()
    : mTexture{},
      mSprite(mTexture),
      mMovementSpeed(100.f),
      maxHealth(100.f),
      currentHealth(100.f)
{

}

// Parameterized constructor
Entity::Entity(float speed, float health)
    : mTexture{},
      mSprite(mTexture),
      mMovementSpeed(speed),
      maxHealth(health),
      currentHealth(health)
{

}

// --- const Getters ---
sf::FloatRect Entity::getGlobalBounds() const {
    return mSprite.getGlobalBounds();
}

sf::Vector2f Entity::getPosition() const {
    return mSprite.getPosition();
}

sf::Vector2u Entity::getSpriteSize() const {
    return mTexture.getSize();
}

float Entity::getCurrentHealth() const {
    return currentHealth;
}

float Entity::getMaxHealth() const {
    return maxHealth;
}

void Entity::render(sf::RenderWindow& window) const {
    window.draw(mSprite);
}

// --- operator<< ---
std::ostream& operator<<(std::ostream& os, const Entity& entity) {
    os << "Entity(Health: " << entity.currentHealth << "/" << entity.maxHealth
       << ", Pos: {" << entity.getPosition().x << ", " << entity.getPosition().y << "}"
       << ", Speed: " << entity.mMovementSpeed << ")";
    return os;
}