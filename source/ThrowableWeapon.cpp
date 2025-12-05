#include "../header/ThrowableWeapon.h"
#include <iostream>
#include <SFML/System/Angle.hpp>
#include <exception>

ThrowableWeapon::ThrowableWeapon(const std::string& name, float damage, float reloadTime, float range, float explosionRadius, float fuseTime, const sf::Texture& texture)
    : Weapon(name, damage, reloadTime, range),
      mExplosionRadius(explosionRadius),
      mFuseTime(fuseTime),
      mCurrentTimer(0.f),
      mIsActive(false),
      mIsExploding(false),
      mHasDamaged(false),
      mFinished(false),
      mGrenadeSprite(texture)
{
    sf::Vector2u size = texture.getSize();
    mGrenadeSprite.setOrigin({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f});
    mGrenadeSprite.setScale({0.5f, 0.5f});

    mExplosionShape.setRadius(mExplosionRadius);
    mExplosionShape.setOrigin({mExplosionRadius, mExplosionRadius});
    mExplosionShape.setFillColor(sf::Color(255, 69, 0, 150));
}

std::unique_ptr<Weapon> ThrowableWeapon::clone() const {
    return std::make_unique<ThrowableWeapon>(*this);
}

void ThrowableWeapon::load(sf::Texture& textureToLoad) {
    try {
        // 1. Load the actual image file into the provided texture reference
        if (!textureToLoad.loadFromFile("../assets/grenade.png")) {
            throw std::runtime_error("Failed to load grenade.png");
        }




        // 2. Re-initialize *this with the newly loaded texture and correct stats
        *this = ThrowableWeapon("Frag Grenade", 150.f, 0.f, 0.f, 200.f, 2.0f, textureToLoad);

        ///set the scale of the grande sprite
        sf::Vector2u size = textureToLoad.getSize();
        mGrenadeSprite.setOrigin({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f});
        mGrenadeSprite.setScale({0.2f, 0.2f});

        std::cout << "ThrowableWeapon loaded successfully.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading ThrowableWeapon: " << e.what() << "\n";
    }
}

void ThrowableWeapon::throwAt(const sf::Vector2f position) {
    mWorldPosition = position;
    mGrenadeSprite.setPosition(mWorldPosition);
    mExplosionShape.setPosition(mWorldPosition);
    mIsActive = true;
    mCurrentTimer = 0.f;
}

void ThrowableWeapon::update(sf::Time deltaTime) {
    if (mFinished || !mIsActive) return;

    mCurrentTimer += deltaTime.asSeconds();

    if (!mIsExploding) {
        mGrenadeSprite.rotate(sf::degrees(360.f * deltaTime.asSeconds()));

        float blinkSpeed = 5.f + (mCurrentTimer * 5.f);
        if (static_cast<int>(mCurrentTimer * blinkSpeed) % 2 == 0) {
            mGrenadeSprite.setColor(sf::Color(255, 100, 100));
        } else {
            mGrenadeSprite.setColor(sf::Color::White);
        }

        if (mCurrentTimer >= mFuseTime) {
            mIsExploding = true;
            mCurrentTimer = 0.f;
        }
    } else {
        if (mCurrentTimer >= 0.5f) {
            mFinished = true;
        }
    }
}

bool ThrowableWeapon::shouldDealDamage() {
    if (mIsExploding && !mHasDamaged) {
        mHasDamaged = true;
        return true;
    }
    return false;
}

void ThrowableWeapon::render(sf::RenderWindow& window) const {
    if (!mIsActive || mFinished) return;

    if (mIsExploding) {
        window.draw(mExplosionShape);
    } else {
        window.draw(mGrenadeSprite);
    }
}

bool ThrowableWeapon::isExploding() const { return mIsExploding; }
bool ThrowableWeapon::isFinished() const { return mFinished; }
float ThrowableWeapon::getExplosionRadius() const { return mExplosionRadius; }
sf::Vector2f ThrowableWeapon::getPosition() const { return mWorldPosition; }