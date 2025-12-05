#ifndef THROWABLEWEAPON_H
#define THROWABLEWEAPON_H

#include "Weapon.h"
#include <SFML/Graphics.hpp>
#include <memory>

class ThrowableWeapon : public Weapon {
public:
    ThrowableWeapon(const std::string& name, float damage, float reloadTime, float range, float explosionRadius, float fuseTime, const sf::Texture& texture);

    std::unique_ptr<Weapon> clone() const override;

    // New load function: Handles loading the texture file and configuring stats
    void load(sf::Texture& textureToLoad);

    void throwAt(sf::Vector2f position);
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window) const override;

    bool isExploding() const;
    bool isFinished() const;
    bool shouldDealDamage();

    float getExplosionRadius() const;
    sf::Vector2f getPosition() const;

private:
    float mExplosionRadius;
    float mFuseTime;
    float mCurrentTimer;

    bool mIsActive;
    bool mIsExploding;
    bool mHasDamaged;
    bool mFinished;

    sf::Sprite mGrenadeSprite;
    sf::CircleShape mExplosionShape;
    sf::Vector2f mWorldPosition;
};

#endif