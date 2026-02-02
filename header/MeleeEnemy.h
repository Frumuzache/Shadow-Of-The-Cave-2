/**
 * @file MeleeEnemy.h
 * @brief Melee enemy class for close-range attacks
 */

#pragma once
#include "Enemy.h"

/**
 * @class MeleeEnemy
 * @brief Specialized melee enemy that attacks in close range
 *
 * Characteristics:
 * - Higher health and movement speed
 * - Close-range melee attacks with high damage
 * - Uses Melee weapon (Zombie Claws)
 * - Charges directly at player
 *
 * @see Enemy, EnemyType
 */
class MeleeEnemy : public Enemy {
public:
    /// @brief Default constructor
    MeleeEnemy();

    /**
     * @brief Parameterized constructor with position
     * @param startPosition Starting position in world
     */
    explicit MeleeEnemy(sf::Vector2f startPosition);

    /**
     * @brief Full parameterized constructor
     * @param startPosition Starting position in world
     * @param speed Movement speed
     * @param health Maximum health
     */
    MeleeEnemy(sf::Vector2f startPosition, float speed, float health);

    /// @brief Virtual destructor
    ~MeleeEnemy() override = default;

private:
};

