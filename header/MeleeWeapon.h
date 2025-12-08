#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H

#include "Weapon.h"

class MeleeWeapon : public Weapon {
public:
    MeleeWeapon();
    MeleeWeapon(const std::string& name, float damage, float reloadTime, float range);

    std::unique_ptr<Weapon> clone() const override;
    void print(std::ostream& os) const override;

    // New load function
    void load();
};

std::ostream& operator<<(std::ostream& os, const MeleeWeapon& weapon);

#endif