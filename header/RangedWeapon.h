#ifndef RANGEDWEAPON_H
#define RANGEDWEAPON_H

#include "Weapon.h"
#include <SFML/Graphics.hpp>

class RangedWeapon final : public Weapon {
public:
    RangedWeapon();
    RangedWeapon(const std::string& name, float damage, float reloadTime, float range);

    std::unique_ptr<Weapon> clone() const override;

    // New load function to handle specific setup
    void load();

    void print(std::ostream& os) const override;
    void applyUpgrade() override;



private:
    // int mMaxAmmo;
    // int mCurrentAmmo;
};

#endif