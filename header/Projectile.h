/**
 * @file Projectile.h
 * @brief Player projectile class
 */

#pragma once
#include <SFML/Graphics.hpp>

/**
 * @class Projectile
 * @brief Player projectile fired by ranged weapon
 *
 * Represents projectiles fired by the player's ranged weapon.
 * Uses object pooling for performance optimization.
 *
 * Visual: Blue circular projectiles
 * @see Pool, EnemyProjectile
 */
class Projectile {
public:
    /**
     * @brief Default constructor for object pooling
     * Creates an inactive projectile
     */
    Projectile();

    /**
     * @brief Parameterized constructor
     * @param startPos Starting position
     * @param direction Direction vector (will be normalized)
     * @param speed Projectile travel speed
     * @param damage Damage dealt on impact
     */
    Projectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, float damage);

    /**
     * @brief Update projectile position and check bounds
     * @param deltaTime Time elapsed since last frame
     * @param mapBounds Game world boundaries
     */
    void update(sf::Time deltaTime, const sf::Vector2f& mapBounds);

    /**
     * @brief Render projectile to window
     * @param window Target render window
     */
    void render(sf::RenderWindow& window) const;

    /**
     * @brief Get projectile bounding box
     * @return FloatRect representing projectile bounds for collision
     */
    sf::FloatRect getBounds() const;

    /// @brief Get damage value
    float getDamage() const { return mDamage; }

    /// @brief Check if projectile should be removed
    bool isDestroyed() const { return mDestroyed; }

    /// @brief Mark projectile for removal
    void destroy() { mDestroyed = true; }

private:
    /// @name Projectile Visual Representation
    /// @{
    sf::CircleShape mShape;         ///< Circular projectile shape
    /// @}

    /// @name Projectile Physics
    /// @{
    sf::Vector2f mVelocity;         ///< Current velocity vector
    float mDamage;                  ///< Damage on impact
    /// @}

    /// @name Projectile State
    /// @{
    bool mDestroyed;                ///< Flag for removal/pooling
    /// @}
};