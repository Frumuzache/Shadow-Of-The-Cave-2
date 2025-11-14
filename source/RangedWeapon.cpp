#include "../header/RangedWeapon.h"
#include <ostream>

// Constructorul implicit apelează constructorul cu parametri al clasei de bază
RangedWeapon::RangedWeapon()
    : Weapon("Basic Bow", 10.f, 1.5f, 300.f),
      mMaxAmmo(20),
      mCurrentAmmo(20) {
}

// Constructorul cu parametri apelează constructorul clasei de bază (Weapon)
RangedWeapon::RangedWeapon(const std::string& name, float damage, float reloadTime, float range, int maxAmmo)
    : Weapon(name, damage, reloadTime, range),
      mMaxAmmo(maxAmmo),
      mCurrentAmmo(maxAmmo) {
}

void RangedWeapon::reload() {
    mCurrentAmmo = mMaxAmmo;
    // Aici s-ar putea adăuga și un timp de reîncărcare
}

int RangedWeapon::getCurrentAmmo() const {
    return mCurrentAmmo;
}

int RangedWeapon::getMaxAmmo() const {
    return mMaxAmmo;
}

// Implementarea operatorului<<
std::ostream& operator<<(std::ostream& os, const RangedWeapon& weapon) {
    // Apelăm operatorul<< al clasei de bază (Weapon)
    os << static_cast<const Weapon&>(weapon);

    // Adăugăm informații specifice acestei clase
    os << " [Type: Ranged, Ammo: " << weapon.mCurrentAmmo << "/" << weapon.mMaxAmmo << "]";
    return os;
}