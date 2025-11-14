#include "../header/MeleeWeapon.h"
#include <ostream>

// Constructorul implicit apelează constructorul cu parametri al clasei de bază
MeleeWeapon::MeleeWeapon()
    : Weapon("Basic Sword", 15.f, 0.8f, 60.f) {
    // Corpul constructorului e gol, inițializarea se face în listă
}

// Constructorul cu parametri apelează constructorul clasei de bază (Weapon)
MeleeWeapon::MeleeWeapon(const std::string& name, float damage, float reloadTime, float range)
    : Weapon(name, damage, reloadTime, range) {
    // Corpul constructorului e gol, inițializarea se face în listă
}

// Implementarea operatorului<<
std::ostream& operator<<(std::ostream& os, const MeleeWeapon& weapon) {
    // Apelăm operatorul<< al clasei de bază (Weapon)
    os << static_cast<const Weapon&>(weapon);

    // Adăugăm informații specifice acestei clase
    os << " [Type: Melee]";
    return os;
}