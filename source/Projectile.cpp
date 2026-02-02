/**
 * @file Projectile.cpp
 * @brief Implementation of the Projectile class
 *
 * Implements player projectiles including:
 * - Projectile movement and physics
 * - Boundary checking
 * - Rendering
 * - Collision detection
 *
 * @see Projectile.h
 */

#include "../header/Projectile.h"

Projectile::Projectile()
    : mDamage(0.f), mDestroyed(false)
{
    mShape.setRadius(5.f);
    mShape.setFillColor(sf::Color::Yellow);
    mShape.setOrigin({5.f, 5.f});
    mShape.setPosition({0.f, 0.f});
    mVelocity = {0.f, 0.f};
}

Projectile::Projectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, float damage)
    : mDamage(damage), mDestroyed(false)
{
    mShape.setRadius(5.f); // Small circle
    mShape.setFillColor(sf::Color::Yellow);
    mShape.setOrigin({5.f, 5.f}); // Center origin
    mShape.setPosition(startPos);

    mVelocity = direction * speed;
}

void Projectile::update(sf::Time deltaTime, const sf::Vector2f& mapBounds) {
    mShape.move(mVelocity * deltaTime.asSeconds());

    sf::Vector2f pos = mShape.getPosition();

    // Destroy if out of map bounds
    if (pos.x < 0 || pos.y < 0 || pos.x > mapBounds.x || pos.y > mapBounds.y) {
        mDestroyed = true;
    }
}

void Projectile::render(sf::RenderWindow& window) const {
    window.draw(mShape);
}

sf::FloatRect Projectile::getBounds() const {
    return mShape.getGlobalBounds();
}