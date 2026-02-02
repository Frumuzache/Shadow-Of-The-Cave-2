/**
 * @file HealthPotion.cpp
 * @brief Implementation of the HealthPotion class
 *
 * Implements consumable health potion functionality:
 * - Creation and initialization
 * - Usage and healing application
 * - Property access
 *
 * @see HealthPotion.h
 */

#include "../header/HealthPotion.h"
#include "../header/Player.h"
#include <iostream>

/**
 * @brief Default constructor
 *
 * Creates a health potion with default values:
 * - Heal amount: 50 HP
 * - Cost: 25 coins
 */
HealthPotion::HealthPotion()
    : mHealAmount(50.f), mCost(25)
{
    std::cout << "HealthPotion created with default values (50 HP, 25 coins)\n";
}

/**
 * @brief Parameterized constructor
 *
 * @param healAmount Amount of health to restore when used
 *
 * Sets cost proportional to healing amount (1 coin per 2 HP)
 */
HealthPotion::HealthPotion(float healAmount)
    : mHealAmount(healAmount), mCost(static_cast<int>(healAmount / 2.f))
{
    std::cout << "HealthPotion created with " << mHealAmount << " HP healing power, cost: " << mCost << " coins\n";
}

/**
 * @brief Use the potion on a player
 *
 * Applies the healing effect to the player character.
 *
 * @param player Reference to player to heal
 * @return true if potion was used successfully, false if player is at full health
 */
bool HealthPotion::use(Player& player) const
{
    // Apply healing to the player
    player.heal(mHealAmount);

    std::cout << "HealthPotion used! Restored " << mHealAmount << " HP\n";
    return true;
}
