/**
 * @file HealthPotion.h
 * @brief Health potion consumable item class
 */

#pragma once

/// Forward declaration
class Player;

/**
 * @class HealthPotion
 * @brief Consumable item that restores player health
 *
 * Represents a single health potion item that can be used to heal the player.
 * Has a fixed healing amount and cost in coins.
 */
class HealthPotion {
public:
    /// @brief Default constructor
    HealthPotion();

    /**
     * @brief Parameterized constructor
     * @param healAmount Amount of health to restore when used
     */
    explicit HealthPotion(float healAmount);

    /**
     * @brief Use the potion on a player
     * @param player Reference to player to heal
     * @return true if potion was used successfully, false otherwise
     */
    bool use(class Player& player) const;

    /// @name Property Getters
    /// @{
    /// @brief Get healing amount of this potion
    float getHealAmount() const { return mHealAmount; }

    /// @brief Get cost in coins
    int getCost() const { return mCost; }

    /// @brief Get potion name
    const char* getName() const { return "Health Potion"; }
    /// @}

private:
    /// @name Properties
    /// @{
    float mHealAmount;              ///< Amount of health to restore
    int mCost;                      ///< Cost in coins to purchase
    /// @}
};
