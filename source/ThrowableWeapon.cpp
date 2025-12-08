#include "../header/ThrowableWeapon.h"
#include <iostream>
#include <SFML/System/Angle.hpp>
#include <exception>

ThrowableWeapon::ThrowableWeapon()
    : Weapon("Basic Grenade", 0.f, 0.f, 0.f, WeaponType::Throwable), // Ensure WeaponType is correct
      mExplosionRadius(0.f),
      mFuseTime(0.f),
      mCurrentTimer(0.f),
      mIsActive(false),
      mIsExploding(false),
      mHasDamaged(false),
      mFinished(false)
{
}

ThrowableWeapon::ThrowableWeapon(const std::string& name, float damage, float reloadTime, float range, float explosionRadius, float fuseTime)
    : Weapon(name, damage, reloadTime, range, WeaponType::Throwable),
      mExplosionRadius(explosionRadius),
      mFuseTime(fuseTime),
      mCurrentTimer(0.f),
      mIsActive(false),
      mIsExploding(false),
      mHasDamaged(false),
      mFinished(false)
{
    // Initialize explosion visual
    mExplosionShape.setRadius(mExplosionRadius);
    mExplosionShape.setOrigin({mExplosionRadius, mExplosionRadius});
    mExplosionShape.setFillColor(sf::Color(255, 69, 0, 150));
}

std::unique_ptr<Weapon> ThrowableWeapon::clone() const {
    return std::make_unique<ThrowableWeapon>(*this);
}

void ThrowableWeapon::load() {
    try {
        // Re-initialize using the assignment pattern
        *this = ThrowableWeapon("Frag Grenade", 150.f, 2.f, 200.f, 100.f, 2.f);

        loadTexture("../assets/grenade.png"); // Make sure this path exists, or use "rifle.png" if testing
        setVisualSize(90.f, 80.f);            // Small size for grenade

        std::cout << "ThrowableWeapon loaded successfully.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading ThrowableWeapon: " << e.what() << "\n";
    }
}

void ThrowableWeapon::print(std::ostream& os) const {
    Weapon::print(os);
    os << " [Type: Throwable | Radius: " << mExplosionRadius << "]";
}

// --- Gameplay Logic ---

void ThrowableWeapon::throwAt(const sf::Vector2f position) {
    mWorldPosition = position;

    // FIX 1: Use mSprite instead of mGrenadeSprite
    mSprite.setPosition(mWorldPosition);
    mExplosionShape.setPosition(mWorldPosition);

    mIsActive = true;
    mCurrentTimer = 0.f;
}

void ThrowableWeapon::update(sf::Time deltaTime) {
    if (mFinished || !mIsActive) return;

    mCurrentTimer += deltaTime.asSeconds();

    if (!mIsExploding) {
        // FIX 2: Use mSprite
        mSprite.rotate(sf::degrees(360.f * deltaTime.asSeconds()));

        // Blink effect
        float blinkSpeed = 5.f + (mCurrentTimer * 5.f);

        // FIX 3: Use mSprite
        if (static_cast<int>(mCurrentTimer * blinkSpeed) % 2 == 0) {
            mSprite.setColor(sf::Color(255, 100, 100));
        } else {
            mSprite.setColor(sf::Color::White);
        }

        if (mCurrentTimer >= mFuseTime) {
            mIsExploding = true;
            mCurrentTimer = 0.f;
        }
    } else {
        // Explosion duration
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
        // FIX 4: Use mSprite
        window.draw(mSprite);
    }
}

void ThrowableWeapon::applyUpgrade() {
    // Increase radius by 25 units
    mExplosionRadius += 25.f;

    // CRITICAL: Update the visual shape to match the new logic
    mExplosionShape.setRadius(mExplosionRadius);

    // Re-center the origin so the explosion stays centered on the grenade
    mExplosionShape.setOrigin({mExplosionRadius, mExplosionRadius});

    std::cout << ">>> THROWABLE UPGRADE: Bigger boom! New Radius: "
              << mExplosionRadius << "\n";
}


bool ThrowableWeapon::isExploding() const { return mIsExploding; }
bool ThrowableWeapon::isFinished() const { return mFinished; }
float ThrowableWeapon::getExplosionRadius() const { return mExplosionRadius; }
sf::Vector2f ThrowableWeapon::getPosition() const { return mWorldPosition; }