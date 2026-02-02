/**
 * @file EnemyProjectile.h
 * @brief Enemy projectile class
 */

#pragma once
#include <SFML/Graphics.hpp>

/**
 * @class EnemyProjectile
 * @brief Projectile fired by ranged enemies
 *
 * Similar to Projectile but with distinct visual characteristics (red color)
 * to distinguish from player projectiles.
 * Uses object pooling for performance optimization.
 *
 * Visual: Red circular projectiles
 * @see Pool, Projectile
 */
class EnemyProjectile {
public:
    /**
     * @brief Default constructor for object pooling
     * Creates an inactive projectile
     */
    EnemyProjectile();

    /**
     * @brief Parameterized constructor
     * @param startPos Starting position
     * @param direction Direction vector (will be normalized)
     * @param speed Projectile travel speed
     * @param damage Damage dealt on impact
     */
    EnemyProjectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, float damage);

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
    sf::CircleShape mShape;         ///< Circular projectile shape (red)
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
