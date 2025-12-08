#ifndef THROWABLEWEAPON_H
#define THROWABLEWEAPON_H

#include "Weapon.h"
#include <SFML/Graphics.hpp>
#include <memory>

class ThrowableWeapon final : public Weapon {
public:
    ThrowableWeapon();
    ThrowableWeapon(const std::string& name, float damage, float reloadTime, float range, float explosionRadius, float fuseTime);

    std::unique_ptr<Weapon> clone() const override;

    // Load function to handle specific setup (matches RangedWeapon style)
    void load();

    // Specific Grenade Functionality
    void throwAt(sf::Vector2f position);
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window) const override;

    bool isExploding() const;
    bool isFinished() const;
    bool shouldDealDamage();

    float getExplosionRadius() const;
    sf::Vector2f getPosition() const;

    void print(std::ostream& os) const override;

private:
    float mExplosionRadius;
    float mFuseTime;
    float mCurrentTimer;

    bool mIsActive;
    bool mIsExploding;
    bool mHasDamaged;
    bool mFinished;

    sf::CircleShape mExplosionShape;
    sf::Vector2f mWorldPosition;
};

#endif