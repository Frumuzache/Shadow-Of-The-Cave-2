#pragma once
#include "Weapon.h"
#include <SFML/System/Time.hpp>

class MagicWeapon : public Weapon {
private:
    bool mIsActive;
    float mTotalDuration;
    float mCurrentDuration;

    float mTickInterval;
    float mTickTimer;
    bool mReadyToDamage;

public:
    MagicWeapon();

    // Activates the spell
    void castSpell();

    // Updates timers
    void update(sf::Time deltaTime);

    // Checks if we should hit enemies this frame
    bool shouldDealDamage();

    // Returns true if the spell is currently running
    bool isActive() const;

    // Virtual overrides
    std::unique_ptr<Weapon> clone() const override;
    void print(std::ostream& os) const override;
    void applyUpgrade() override;
};