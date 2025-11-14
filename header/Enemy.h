#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <ostream> // For std::ostream
#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy();
    // Parameterized constructor
    Enemy(sf::Vector2f startPosition, float speed, float health);

    void update(sf::Time deltaTime, const sf::Window& window) override;
    void takeDamage(float damageAmount);
    static void death(); // This being static is a bit odd, but kept it

    // operator<< for display
    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy);

private:
    // Removed UpdateHealthEnemy as it was dead code (logic is in Game.cpp)
    void updateMovementEnemy(sf::Time deltaTime, const sf::Window& window);
    sf::Vector2f mDirection;
    sf::Vector2f initialPosition;
};

#endif //ENEMY_H