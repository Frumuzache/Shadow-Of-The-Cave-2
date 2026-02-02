/**
 * @file MeleeEnemy.cpp
 * @brief Implementation of the MeleeEnemy class
 *
 * Implements melee-specific enemy behavior:
 * - Higher health and speed
 * - Close-range melee weapon
 * - Direct charging at player
 *
 * @see MeleeEnemy.h, Enemy.h
 */

#include "../header/MeleeEnemy.h"

// Default Constructor
MeleeEnemy::MeleeEnemy()
    : Enemy(sf::Vector2f(100.f, 100.f), 200.f, 80.f, EnemyType::Melee)
{
    // MeleeEnemy specifics handled by parent class with Melee type
}

// Constructor with position
MeleeEnemy::MeleeEnemy(sf::Vector2f startPosition)
    : Enemy(startPosition, 200.f, 80.f, EnemyType::Melee)
{
    // MeleeEnemy specifics handled by parent class with Melee type
}

// Fully customizable constructor
MeleeEnemy::MeleeEnemy(sf::Vector2f startPosition, float speed, float health)
    : Enemy(startPosition, speed, health, EnemyType::Melee)
{
    // MeleeEnemy specifics handled by parent class with Melee type
}
