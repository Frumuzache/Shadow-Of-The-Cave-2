#pragma once

/**
 * @class HealthPotion
 * @brief Consumable item that restores player health
 */
class HealthPotion {
public:
    HealthPotion();
    explicit HealthPotion(float healAmount);

    // Use the potion (returns true if used successfully)
    bool use(class Player& player) const;

    // Getters
    float getHealAmount() const { return mHealAmount; }
    int getCost() const { return mCost; }
    const char* getName() const { return "Health Potion"; }

private:
    float mHealAmount;  // Amount of health to restore
    int mCost;         // Cost in coins
};
