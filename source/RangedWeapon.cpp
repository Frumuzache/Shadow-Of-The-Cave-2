#include "../header/RangedWeapon.h"
#include <iostream>
#include <exception>

RangedWeapon::RangedWeapon()
    : Weapon("Basic Bow", 10.f, 1.5f, 300.f),
      mMaxAmmo(20),
      mCurrentAmmo(20) {
}

RangedWeapon::RangedWeapon(const std::string& name, float damage, float reloadTime, float range, int maxAmmo)
    : Weapon(name, damage, reloadTime, range),
      mMaxAmmo(maxAmmo),
      mCurrentAmmo(maxAmmo) {
}

std::unique_ptr<Weapon> RangedWeapon::clone() const {
    return std::make_unique<RangedWeapon>(*this);
}

void RangedWeapon::load() {
    try {

        *this = RangedWeapon("AK-47", 15.f, 0.1f, 800.f, 30);
        loadTexture("../assets/rifle.png");
        setVisualSize(200.f, 100.f);
        std::cout << "RangedWeapon loaded successfully.\n";
    }

    catch (const std::exception& e) {
        std::cerr << "Error loading RangedWeapon: " << e.what() << "\n";
    }
}

std::ostream& operator<<(std::ostream& os, const RangedWeapon& weapon) {
    os << static_cast<const Weapon&>(weapon);
    os << " [Type: Ranged]";
    return os;
}