#ifndef RANGEDWEAPON_H
#define RANGEDWEAPON_H
#include "Weapon.h"

class RangedWeapon final : public Weapon {
public:
    // Constructor implicit
    RangedWeapon();

    // Constructor cu parametri
    RangedWeapon(const std::string& name, float damage, float reloadTime, float range, int maxAmmo);

    // Funcții specifice armelor ranged
    // void reload();
    // int getCurrentAmmo() const;
    // int getMaxAmmo() const;

    // Suprascriem funcția de atac (poate)
    // float attack() const; // (momentan folosim cea de bază)

    // Suprascriem operatorul<< pentru afișare
    friend std::ostream& operator<<(std::ostream& os, const RangedWeapon& weapon);

private:
    int mMaxAmmo;
    int mCurrentAmmo;
};

#endif //RANGEDWEAPON_H