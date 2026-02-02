#pragma once
#include <SFML/Graphics.hpp>

/**
 * @class EnemyProjectile
 * @brief Projectile fired by ranged enemies (RED color)
 *
 * Similar to Projectile but with red color to distinguish from player projectiles.
 */
class EnemyProjectile {
public:
    EnemyProjectile();  // Default constructor for pooling
    EnemyProjectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, float damage);

    void update(sf::Time deltaTime, const sf::Vector2f& mapBounds);
    void render(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const;
    float getDamage() const { return mDamage; }
    bool isDestroyed() const { return mDestroyed; }
    void destroy() { mDestroyed = true; }

private:
    sf::CircleShape mShape;
    sf::Vector2f mVelocity;
    float mDamage;
    bool mDestroyed;
};
