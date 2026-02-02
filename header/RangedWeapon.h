/**
 * @file RangedWeapon.h
 * @brief Ranged weapon class (rifle)
 */

#ifndef RANGEDWEAPON_H
#define RANGEDWEAPON_H

#include "Weapon.h"
#include <SFML/Graphics.hpp>

/**
 * @class RangedWeapon
 * @brief Long-range projectile weapon (rifle)
 *
 * Specialized weapon for ranged attacks with characteristics:
 * - Moderate damage, moderate reload time
 * - Long range
 * - Fires projectiles
 * - Used by player as primary weapon
 */
class RangedWeapon final : public Weapon {
public:
    /// @brief Default constructor
    RangedWeapon();

    /**
     * @brief Parameterized constructor
     * @param name Weapon name (e.g., "Rifle")
     * @param damage Damage per hit
     * @param reloadTime Cooldown between attacks
     * @param range Attack range in pixels
     */
    RangedWeapon(const std::string& name, float damage, float reloadTime, float range);

    /// @brief Create a copy of this ranged weapon
    std::unique_ptr<Weapon> clone() const override;

    /// @brief Load ranged weapon assets
    void load();

    /// @brief Print ranged weapon info to stream
    void print(std::ostream& os) const override;

    /// @brief Apply damage upgrade to this weapon
    void applyUpgrade() override;


private:
    // int mMaxAmmo;
    // int mCurrentAmmo;
};

#endif