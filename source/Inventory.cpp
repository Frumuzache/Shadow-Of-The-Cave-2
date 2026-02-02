/**
 * @file Inventory.cpp
 * @brief Implementation of the Inventory class
 *
 * Implements inventory management including:
 * - Adding and removing consumable items
 * - Item usage and constraints
 * - Quantity tracking (max 99 per item type)
 *
 * @see Inventory.h
 */

#include "../header/Inventory.h"
#include <algorithm>
#include <iostream>
#include "../header/MathHelper.h"

Inventory::Inventory()
    : mHealthPotions(0), mGrenades(0), mDamagePotions(0)
{
}

void Inventory::addHealthPotion(int count) {
    // ✨ TEMPLATE FUNCTION INSTANTIATION 6: clamp<int>
    mHealthPotions = MathHelper::clamp<int>(mHealthPotions + count, 0, MAX_ITEMS);
    std::cout << "Added " << count << " Health Potion(s). Total: " << mHealthPotions << "\n";
}

void Inventory::removeHealthPotion(int count) {
    // ✨ TEMPLATE FUNCTION INSTANTIATION 7: clamp<int>
    mHealthPotions = MathHelper::clamp<int>(mHealthPotions - count, 0, MAX_ITEMS);
    std::cout << "Used Health Potion. Remaining: " << mHealthPotions << "\n";
}

void Inventory::addGrenade(int count) {
    // ✨ TEMPLATE FUNCTION INSTANTIATION 8: clamp<int>
    mGrenades = MathHelper::clamp<int>(mGrenades + count, 0, MAX_ITEMS);
    std::cout << "Added " << count << " Grenade(s). Total: " << mGrenades << "\n";
}

void Inventory::removeGrenade(int count) {
    // ✨ TEMPLATE FUNCTION INSTANTIATION 9: clamp<int>
    mGrenades = MathHelper::clamp<int>(mGrenades - count, 0, MAX_ITEMS);
    std::cout << "Used Grenade. Remaining: " << mGrenades << "\n";
}

void Inventory::addDamagePotion(int count) {
    // ✨ TEMPLATE FUNCTION INSTANTIATION 10: clamp<int>
    mDamagePotions = MathHelper::clamp<int>(mDamagePotions + count, 0, MAX_ITEMS);
    std::cout << "Added " << count << " Damage Potion(s). Total: " << mDamagePotions << "\n";
}

void Inventory::removeDamagePotion(int count) {
    // ✨ TEMPLATE FUNCTION INSTANTIATION 11: clamp<int>
    mDamagePotions = MathHelper::clamp<int>(mDamagePotions - count, 0, MAX_ITEMS);
    std::cout << "Used Damage Potion. Remaining: " << mDamagePotions << "\n";
}

bool Inventory::useHealthPotion() {
    if (mHealthPotions > 0) {
        --mHealthPotions;
        return true;
    }
    return false;
}

bool Inventory::useGrenade() {
    if (mGrenades > 0) {
        --mGrenades;
        return true;
    }
    return false;
}

bool Inventory::useDamagePotion() {
    if (mDamagePotions > 0) {
        --mDamagePotions;
        return true;
    }
    return false;
}
