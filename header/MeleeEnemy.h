#pragma once
#include "Enemy.h"

/**
 * @class MeleeEnemy
 * @brief Specialized melee enemy that attacks in close range
 *
 * Characteristics:
 * - Higher health and speed
 * - Close-range melee attacks with high damage
 * - Uses Melee weapon (Zombie Claws)
 */
class MeleeEnemy : public Enemy {
public:
    // Constructors
    MeleeEnemy();
    explicit MeleeEnemy(sf::Vector2f startPosition);
    MeleeEnemy(sf::Vector2f startPosition, float speed, float health);

    // Destructor
    ~MeleeEnemy() override = default;

private:
};
