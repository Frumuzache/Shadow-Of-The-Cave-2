#pragma once

/**
 * @class Inventory
 * @brief Simple inventory system for consumable items
 */
class Inventory {
public:
    Inventory();

    // Add/Remove items
    void addHealthPotion(int count = 1);
    void removeHealthPotion(int count = 1);
    void addGrenade(int count = 1);
    void removeGrenade(int count = 1);
    void addDamagePotion(int count = 1);
    void removeDamagePotion(int count = 1);

    // Getters
    int getHealthPotionCount() const { return mHealthPotions; }
    int getGrenadeCount() const { return mGrenades; }
    int getDamagePotionCount() const { return mDamagePotions; }

    // Use items
    bool useHealthPotion();
    bool useGrenade();
    bool useDamagePotion();

private:
    int mHealthPotions;
    int mGrenades;
    int mDamagePotions;
    const int MAX_ITEMS = 99;  // Max per type
};
