#ifndef RANGEDWEAPON_H
#define RANGEDWEAPON_H

#include "Weapon.h"
#include <SFML/Graphics.hpp>

class RangedWeapon : public Weapon {
public:
    RangedWeapon();
    RangedWeapon(const std::string& name, float damage, float reloadTime, float range, int maxAmmo);

    std::unique_ptr<Weapon> clone() const override;

    // New load function to handle specific setup
    void load();

private:
    // int mMaxAmmo;
    // int mCurrentAmmo;
};

// Operator overload for printing
std::ostream& operator<<(std::ostream& os, const RangedWeapon& weapon);

#endif