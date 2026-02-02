/**
 * @file Inventory.h
 * @brief Inventory system for consumable items
 */

#pragma once

/**
 * @class Inventory
 * @brief Simple inventory system for tracking consumable items
 *
 * Manages:
 * - Health potions (healing items)
 * - Grenades (throwable weapons)
 * - Damage potions (damage boosters)
 *
 * Enforces maximum item limits (99 per type).
 */
class Inventory {
public:
    /// @brief Default constructor
    Inventory();

    /// @name Health Potion Management
    /// @{
    /**
     * @brief Add health potions to inventory
     * @param count Number of potions to add (default: 1)
     */
    void addHealthPotion(int count = 1);

    /**
     * @brief Remove health potions from inventory
     * @param count Number of potions to remove (default: 1)
     */
    void removeHealthPotion(int count = 1);
    /// @}

    /// @name Grenade Management
    /// @{
    /**
     * @brief Add grenades to inventory
     * @param count Number of grenades to add (default: 1)
     */
    void addGrenade(int count = 1);

    /**
     * @brief Remove grenades from inventory
     * @param count Number of grenades to remove (default: 1)
     */
    void removeGrenade(int count = 1);
    /// @}

    /// @name Damage Potion Management
    /// @{
    /**
     * @brief Add damage potions to inventory
     * @param count Number of damage potions to add (default: 1)
     */
    void addDamagePotion(int count = 1);

    /**
     * @brief Remove damage potions from inventory
     * @param count Number of damage potions to remove (default: 1)
     */
    void removeDamagePotion(int count = 1);
    /// @}

    /// @name Inventory Queries
    /// @{
    /// @brief Get count of health potions
    [[nodiscard]] int getHealthPotionCount() const { return mHealthPotions; }

    /// @brief Get count of grenades
    [[nodiscard]] int getGrenadeCount() const { return mGrenades; }

    /// @brief Get count of damage potions
    [[nodiscard]] int getDamagePotionCount() const { return mDamagePotions; }
    /// @}

    /// @name Item Usage
    /// @{
    /**
     * @brief Use a health potion
     * @return true if potion was used, false if none available
     */
    bool useHealthPotion();

    /**
     * @brief Use a grenade
     * @return true if grenade was used, false if none available
     */
    bool useGrenade();

    /**
     * @brief Use a damage potion
     * @return true if damage potion was used, false if none available
     */
    bool useDamagePotion();
    /// @}

private:
    /// @name Inventory Items
    /// @{
    int mHealthPotions;              ///< Count of health potions
    int mGrenades;                   ///< Count of grenades
    int mDamagePotions;              ///< Count of damage potions
    /// @}

    /// @name Constraints
    /// @{
    const int MAX_ITEMS = 99;        ///< Maximum items per type
    /// @}
};

