/**
 * @file MeleeWeapon.h
 * @brief Melee weapon class (sword/claws)
 */

#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

#include "Weapon.h"

/**
 * @class MeleeWeapon
 * @brief Close-range melee weapon (sword)
 *
 * Specialized weapon for melee attacks with characteristics:
 * - High damage, quick reload time
 * - Short range
 * - Used by both player and enemies
 */
class MeleeWeapon : public Weapon {
public:
    /// @brief Default constructor
    MeleeWeapon();

    /**
     * @brief Parameterized constructor
     * @param name Weapon name (e.g., "Sword")
     * @param damage Damage per hit
     * @param reloadTime Cooldown between attacks
     * @param range Attack range in pixels
     */
    MeleeWeapon(const std::string& name, float damage, float reloadTime, float range);

    /// @brief Create a copy of this melee weapon
    std::unique_ptr<Weapon> clone() const override;

    /// @brief Print melee weapon info to stream
    void print(std::ostream& os) const override;

    /// @brief Load melee weapon assets
    void load();

    /// @brief Apply damage upgrade to this weapon
    void applyUpgrade() override;
};


#endif