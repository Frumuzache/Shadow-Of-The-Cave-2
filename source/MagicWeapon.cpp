/**
 * @file MagicWeapon.cpp
 * @brief Implementation of the MagicWeapon class
 *
 * Implements magic spell functionality:
 * - Area damage around player
 * - Duration-based spell casting
 * - Tick-based damage application
 * - Weapon cloning and upgrades
 *
 * @see MagicWeapon.h
 */

#include "../header/MagicWeapon.h"
#include <iostream>

MagicWeapon::MagicWeapon()
    : Weapon("Void Spell", 5.f, 15.f, 9999.f, WeaponType::Magic), // 5 dmg, 15s reload, Infinite range
      mIsActive(false),
      mTotalDuration(10.f),
      mCurrentDuration(0.f),
      mTickInterval(1.0f),
      mTickTimer(0.f),
      mReadyToDamage(false)
{
}

void MagicWeapon::castSpell() {
    if (mIsActive) return; // Cannot cast while active

    mIsActive = true;
    mCurrentDuration = 0.f;
    mTickTimer = 0.f;
    mReadyToDamage = true; // Deal immediate damage on cast

    std::cout << ">>> MAGIC ACTIVATED: Void Spell cast! (10s Duration)\n";
}

void MagicWeapon::update(sf::Time deltaTime) {
    if (!mIsActive) return;

    float dt = deltaTime.asSeconds();
    mCurrentDuration += dt;
    mTickTimer += dt;

    // Check if the spell duration has ended
    if (mCurrentDuration >= mTotalDuration) {
        mIsActive = false;
        mReadyToDamage = false;
        std::cout << ">>> MAGIC ENDED: Void Spell faded.\n";
        return;
    }

    // Check if a second has passed to deal damage again
    if (mTickTimer >= mTickInterval) {
        mTickTimer -= mTickInterval; // Reset tick timer (keep remainder for precision)
        mReadyToDamage = true;
    }
}

bool MagicWeapon::shouldDealDamage() {
    if (mReadyToDamage) {
        mReadyToDamage = false; // Reset flag so we don't damage every frame
        return true;
    }
    return false;
}

bool MagicWeapon::isActive() const {
    return mIsActive;
}

std::unique_ptr<Weapon> MagicWeapon::clone() const {
    return std::make_unique<MagicWeapon>(*this);
}

void MagicWeapon::applyUpgrade() {
    this->damage += 2.f; // Upgrade: +2 Damage per tick
    std::cout << ">>> MAGIC UPGRADE: Void Spell intensified! New Dmg/Sec: " << this->damage << "\n";
}

void MagicWeapon::print(std::ostream& os) const {
    Weapon::print(os);
    os << " [Type: Magic | Duration: " << mTotalDuration << "s]";
}