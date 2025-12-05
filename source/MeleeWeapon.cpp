#include "../header/MeleeWeapon.h"
#include <iostream>
#include <exception>

MeleeWeapon::MeleeWeapon()
    : Weapon("Basic Sword", 15.f, 0.8f, 60.f) {
}

MeleeWeapon::MeleeWeapon(const std::string& name, float damage, float reloadTime, float range)
    : Weapon(name, damage, reloadTime, range) {
}

std::unique_ptr<Weapon> MeleeWeapon::clone() const {
    return std::make_unique<MeleeWeapon>(*this);
}

void MeleeWeapon::load() {
    try {
        // Configure specific stats for the default Melee Weapon
        *this = MeleeWeapon("Combat Knife", 30.f, 0.5f, 60.f);

        // Optional: Load texture if available
        // if (!loadTexture("../assets/knife.png")) { ... }

        std::cout << "MeleeWeapon loaded successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error loading MeleeWeapon: " << e.what() << "\n";
    }
}

std::ostream& operator<<(std::ostream& os, const MeleeWeapon& weapon) {
    os << static_cast<const Weapon&>(weapon);
    os << " [Type: Melee]";
    return os;
}