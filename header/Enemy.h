#pragma once
#include "Entity.h"
#include "MeleeWeapon.h"
#include <SFML/System/Clock.hpp>

class Enemy : public Entity {
public:
    // Constructors
    Enemy();
    Enemy(sf::Vector2f startPosition, float speed, float health);

    // Public Methods
    void update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) override;
    void takeDamage(float damageAmount);
    static void death();
    void render(sf::RenderWindow &window) const override;

    // Debugging
    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy);

private:
    // Private Helper Methods
    void updateMovementEnemy(sf::Time deltaTime, const sf::Vector2f& mapBounds);
    void loadAssets();
    void tryAttack(); // Logic to check range and deal damage

    // Private Attributes
    sf::Vector2f mDirection;
    sf::Vector2f initialPosition;

    // Combat Attributes (Private)
    MeleeWeapon mWeapon;    // The enemy's weapon
    sf::Clock mAttackClock; // Tracks time since last attack
};