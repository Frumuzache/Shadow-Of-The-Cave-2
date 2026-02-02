/**
 * @file RangedEnemy.h
 * @brief Ranged enemy class for long-distance attacks
 */

#pragma once
#include "Enemy.h"

/**
 * @class RangedEnemy
 * @brief Specialized ranged enemy that attacks from distance
 *
 * Characteristics:
 * - Lower health, slower movement speed
 * - Long-range ranged attacks with moderate damage
 * - Uses Ranged weapon (Skeleton Bow)
 * - Maintains distance from player
 *
 * @see Enemy, EnemyType
 */
class RangedEnemy : public Enemy {
public:
    /// @brief Default constructor
    RangedEnemy();

    /**
     * @brief Parameterized constructor with position
     * @param startPosition Starting position in world
     */
    explicit RangedEnemy(sf::Vector2f startPosition);

    /**
     * @brief Full parameterized constructor
     * @param startPosition Starting position in world
     * @param speed Movement speed
     * @param health Maximum health
     */
    RangedEnemy(sf::Vector2f startPosition, float speed, float health);

    /// @brief Virtual destructor
    ~RangedEnemy() override = default;

private:
    /**
     * @brief Initialize rifle weapon with extended range
     * Custom setup for ranged weapon
     */
    void initializeRifle();
};

