/**
 * @file MagicWeapon.h
 * @brief Magic weapon class for spellcasting
 */

#pragma once
#include "Weapon.h"
#include <SFML/System/Time.hpp>

/**
 * @class MagicWeapon
 * @brief Magic spell weapon for magical attacks
 *
 * Specialized weapon for casting spells with characteristics:
 * - Moderate damage, long reload time
 * - Area damage around player
 * - Activates through spellcasting
 * - Deals damage over duration
 */
class MagicWeapon : public Weapon {
private:
    /// @name Spell State
    /// @{
    bool mIsActive;              ///< Spell is currently active
    float mTotalDuration;        ///< Total spell duration
    float mCurrentDuration;      ///< Remaining spell duration
    /// @}

    /// @name Damage Timing
    /// @{
    float mTickInterval;         ///< Time between damage ticks
    float mTickTimer;            ///< Timer for damage ticks
    bool mReadyToDamage;         ///< Ready to deal damage this frame
    /// @}

public:
    /// @brief Default constructor
    MagicWeapon();

    /**
     * @brief Cast the spell
     *
     * Activates the magic weapon and initiates damage sequence.
     */
    void castSpell();

    /**
     * @brief Update spell timers
     * @param deltaTime Time elapsed since last frame
     */
    void update(sf::Time deltaTime);

    /**
     * @brief Check if spell should deal damage this frame
     * @return true if a damage tick should be applied
     */
    bool shouldDealDamage();

    /**
     * @brief Check if spell is currently active
     * @return true if spell is running
     */
    bool isActive() const;

    /// @name Virtual Overrides
    /// @{
    /// @brief Create a copy of this magic weapon
    std::unique_ptr<Weapon> clone() const override;

    /// @brief Print magic weapon info to stream
    void print(std::ostream& os) const override;

    /// @brief Apply damage upgrade to this weapon
    void applyUpgrade() override;
    /// @}
};