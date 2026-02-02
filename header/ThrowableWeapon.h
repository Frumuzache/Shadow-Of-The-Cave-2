/**
 * @file ThrowableWeapon.h
 * @brief Throwable weapon class (grenades)
 */

#ifndef THROWABLEWEAPON_H
#define THROWABLEWEAPON_H

#include "Weapon.h"
#include <SFML/Graphics.hpp>
#include <memory>

/**
 * @class ThrowableWeapon
 * @brief Throwable explosive weapon (grenades)
 *
 * Specialized weapon for throwing projectiles with characteristics:
 * - Moderate damage, slow reload
 * - Medium range
 * - Explodes on impact or after fuse time
 * - Deals area damage on explosion
 */
class ThrowableWeapon final : public Weapon {
public:
    /// @brief Default constructor
    ThrowableWeapon();

    /**
     * @brief Parameterized constructor
     * @param name Weapon name (e.g., "Grenade")
     * @param damage Damage per hit
     * @param reloadTime Cooldown between throws
     * @param range Attack range in pixels
     * @param explosionRadius Explosion area radius
     * @param fuseTime Time before explosion in seconds
     */
    ThrowableWeapon(const std::string& name, float damage, float reloadTime, float range,
                    float explosionRadius, float fuseTime);

    /// @brief Create a copy of this throwable weapon
    std::unique_ptr<Weapon> clone() const override;

    /// @brief Load throwable weapon assets
    void load();

    /// @name Grenade Functionality
    /// @{
    /**
     * @brief Throw grenade at target position
     * @param position Target position for grenade
     */
    void throwAt(sf::Vector2f position);

    /**
     * @brief Update grenade state and timers
     * @param deltaTime Time elapsed since last frame
     */
    void update(sf::Time deltaTime);

    /**
     * @brief Render grenade sprite
     * @param window Target render window
     */
    void render(sf::RenderWindow& window) const override;

    /**
     * @brief Check if grenade is currently exploding
     * @return true if explosion is active
     */
    bool isExploding() const;

    /**
     * @brief Check if grenade has finished (explosion ended)
     * @return true if grenade lifecycle is complete
     */
    bool isFinished() const;

    /**
     * @brief Check if grenade should deal damage this frame
     * @return true if explosion damage should be applied
     */
    bool shouldDealDamage();

    /**
     * @brief Get explosion radius
     * @return Radius of explosion area in pixels
     */
    float getExplosionRadius() const;

    /**
     * @brief Get grenade current position
     * @return Vector2f with grenade coordinates
     */
    sf::Vector2f getPosition() const;
    /// @}

    /// @brief Print throwable weapon info to stream
    void print(std::ostream& os) const override;

    /// @brief Apply damage upgrade to this weapon
    void applyUpgrade() override;

private:
    /// @name Explosion Properties
    /// @{
    float mExplosionRadius;      ///< Radius of explosion effect
    float mFuseTime;             ///< Time before automatic detonation
    float mCurrentTimer;         ///< Current fuse timer
    /// @}

    /// @name State Flags
    /// @{
    bool mIsActive;              ///< Grenade is active/thrown
    bool mIsExploding;           ///< Explosion is currently happening
    bool mHasDamaged;            ///< Damage has been applied this explosion
    bool mFinished;              ///< Grenade lifecycle complete
    /// @}

    /// @name Rendering
    /// @{
    sf::CircleShape mExplosionShape;  ///< Visual representation of explosion
    sf::Vector2f mWorldPosition;      ///< Grenade position in world
    /// @}
};

#endif