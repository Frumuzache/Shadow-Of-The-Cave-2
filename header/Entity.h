#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <ostream> // For std::ostream

class Entity {
public:
    Entity();
    // Parameterized constructor
    Entity(float speed, float health);
    virtual ~Entity() = default;

    virtual void update(sf::Time deltaTime, const sf::Window& window) = 0;

    void render(sf::RenderWindow& window) const;

    // Helper functions (all const)
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2u getSpriteSize() const;

    // Added a base getter for health
    virtual float getCurrentHealth() const;
    float getMaxHealth() const;

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