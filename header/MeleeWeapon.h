#ifndef MELEEWEAPON_H
#define MELEEWEAPON_H
#include "Weapon.h"

class MeleeWeapon final : public Weapon {
public:
    // Constructor implicit
    MeleeWeapon();

    // Constructor cu parametri
    MeleeWeapon(const std::string& name, float damage, float reloadTime, float range);

    // Suprascriem operatorul<< pentru afișare
    friend std::ostream& operator<<(std::ostream& os, const MeleeWeapon& weapon);

private:
    // Aici ar putea fi adăugate proprietăți specifice
    // de ex: float swingSpeed;
};

#endif //MELEEWEAPON_H