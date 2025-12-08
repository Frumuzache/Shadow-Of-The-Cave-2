#include "../header/RangedWeapon.h"
#include <iostream>
#include <exception>

RangedWeapon::RangedWeapon()
    : Weapon("Basic Bow", 10.f, 1.5f, 300.f)
      {}

RangedWeapon::RangedWeapon(const std::string& name, const float damage, const float reloadTime, const float range)
    : Weapon(name, damage, reloadTime, range)
      {}

std::unique_ptr<Weapon> RangedWeapon::clone() const {
    return std::make_unique<RangedWeapon>(*this);
}

void RangedWeapon::load() {
    try {

        *this = RangedWeapon("AK-47", 15.f, 0.1f, 800.f);
        loadTexture("../assets/rifle.png");
        setVisualSize(200.f, 100.f);
        std::cout << "RangedWeapon loaded successfully.\n";
    }

    catch (const std::exception& e) {
        std::cerr << "Error loading RangedWeapon: " << e.what() << "\n";
    }
}

void RangedWeapon::print(std::ostream& os) const {
    // 1. Call base printer first
    Weapon::print(os);
    // 2. Add specific details
    os << " [Type: Ranged]";

}