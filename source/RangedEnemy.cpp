/**
 * @file RangedEnemy.cpp
 * @brief Implementation of the RangedEnemy class
 *
 * Implements ranged-specific enemy behavior:
 * - Lower health, slower movement
 * - Long-range ranged weapon
 * - Maintains distance from player
 * - Fires projectiles at player
 *
 * @see RangedEnemy.h, Enemy.h
 */

#include "../header/RangedEnemy.h"
#include "../header/RangedWeapon.h"
#include <memory>
#include <iostream>

// Default Constructor
RangedEnemy::RangedEnemy()
    : Enemy(sf::Vector2f(100.f, 100.f), 180.f, 50.f, EnemyType::Ranged)
{
    // Override weapon with rifle (longer range)
    initializeRifle();
}

// Constructor with position
RangedEnemy::RangedEnemy(sf::Vector2f startPosition)
    : Enemy(startPosition, 180.f, 50.f, EnemyType::Ranged)
{
    // Override weapon with rifle (longer range)
    initializeRifle();
}

// Fully customizable constructor
RangedEnemy::RangedEnemy(sf::Vector2f startPosition, float speed, float health)
    : Enemy(startPosition, speed, health, EnemyType::Ranged)
{
    // Override weapon with rifle (longer range)
    initializeRifle();
}

// Initialize rifle weapon with extended range
void RangedEnemy::initializeRifle() {
    // Create rifle with custom stats
    // First: Create with desired range (700px - reduced) and damage (10 - reduced)
    auto rifle = std::make_unique<RangedWeapon>("Enemy Rifle", 10.f, 0.6f, 700.f);

    // Load the texture (this will reload some stats)
    rifle->load();

    // NOW: Override the stats that load() changed
    // load() overwrites everything with "AK-47", 15.f, 0.2f, 800.f
    // So we need to set them back to our desired values
    *rifle = RangedWeapon("Enemy Rifle", 10.f, 0.6f, 700.f);

    // Then load texture again (but don't let load() overwrite stats)
    rifle->loadTexture("../assets/rifle.png");

    // Mărește dimensiunea armei (player size: 200x100)
    rifle->setVisualSize(200.f, 100.f);  // Same size as player weapon

    // Assign to mWeapon
    mWeapon = std::move(rifle);

    std::cout << "RangedEnemy initialized with rifle. Range: " << mWeapon->getRange() << "px | Damage: " << mWeapon->getDamage() << " | Reload: " << mWeapon->getReloadTime() << "s\n";
}
