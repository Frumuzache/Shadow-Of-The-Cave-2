#pragma once
#include "Enemy.h"

/**
 * @class RangedEnemy
 * @brief Specialized ranged enemy that attacks from distance
 *
 * Characteristics:
 * - Lower health, slower movement
 * - Long-range ranged attacks with moderate damage
 * - Uses Ranged weapon (Skeleton Bow)
 * - Maintains distance from player
 */
class RangedEnemy : public Enemy {
public:
    // Constructors
    RangedEnemy();
    explicit RangedEnemy(sf::Vector2f startPosition);
    RangedEnemy(sf::Vector2f startPosition, float speed, float health);

    // Destructor
    ~RangedEnemy() override = default;

private:
    // Initialize rifle weapon with extended range
    void initializeRifle();
};
