#include "../header/EnemyProjectile.h"

EnemyProjectile::EnemyProjectile()
    : mDamage(0.f), mDestroyed(false)
{
    mShape.setRadius(5.f);
    mShape.setFillColor(sf::Color::Red);
    mShape.setOrigin({5.f, 5.f});
    mShape.setPosition({0.f, 0.f});
    mVelocity = {0.f, 0.f};
}

EnemyProjectile::EnemyProjectile(sf::Vector2f startPos, sf::Vector2f direction, float speed, float damage)
    : mDamage(damage), mDestroyed(false)
{
    mShape.setRadius(5.f); // Small circle
    mShape.setFillColor(sf::Color::Red); // RED for enemy projectiles!
    mShape.setOrigin({5.f, 5.f}); // Center origin
    mShape.setPosition(startPos);

    mVelocity = direction * speed;
}

void EnemyProjectile::update(sf::Time deltaTime, const sf::Vector2f& mapBounds) {
    mShape.move(mVelocity * deltaTime.asSeconds());

    sf::Vector2f pos = mShape.getPosition();

    // Destroy if out of map bounds
    if (pos.x < 0 || pos.y < 0 || pos.x > mapBounds.x || pos.y > mapBounds.y) {
        mDestroyed = true;
    }
}

void EnemyProjectile::render(sf::RenderWindow& window) const {
    window.draw(mShape);
}

sf::FloatRect EnemyProjectile::getBounds() const {
    return mShape.getGlobalBounds();
}
