#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity();
    // Parameterized constructor
    Entity(float speed, float health);
    virtual ~Entity() = default;

    virtual void update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) = 0;

    virtual void render(sf::RenderWindow& window) const = 0;

    // Helper functions (all const)
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f setPosition(const sf::Vector2f& position);

    sf::Vector2u getSpriteSize() const;

    // Added a base getter for health
    virtual float getCurrentHealth() const;
    // float getMaxHealth() const;

    // Damage handling
    virtual void takeDamage(float damage);

    // operator<< for display
    friend std::ostream& operator<<(std::ostream& os, const Entity& entity);

protected:
    sf::Texture mTexture;
    sf::Sprite mSprite;
    float mMovementSpeed;
    float maxHealth;
    float currentHealth;
};

#endif //ENTITY_H