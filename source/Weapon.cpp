#include "../header/Weapon.h"
#include <iostream>

// Default constructor
Weapon::Weapon()
    : name("Fists"), damage(1.f), reloadTime(0.5f), range(10.f) {
    std::cout << "Weapon default constructor called.\n";
}

// Parameterized constructor
Weapon::Weapon(std::string name, float damage, float reloadTime, float range)
    : name(std::move(name)), damage(damage), reloadTime(reloadTime), range(range) {
    std::cout << "Weapon parameterized constructor called for: " << this->name << "\n";
}

// 1. Destructor
Weapon::~Weapon() {
    std::cout << "Weapon destructor called for: " << name << "\n";
}

// 2. Copy Constructor
Weapon::Weapon(const Weapon& other)
    : name(other.name), damage(other.damage), reloadTime(other.reloadTime), range(other.range) {
    std::cout << "Weapon COPY CONSTRUCTOR called to copy: " << other.name << "\n";
}

// 3. Copy Assignment Operator
Weapon& Weapon::operator=(const Weapon& other) {
    std::cout << "Weapon COPY ASSIGNMENT called to copy: " << other.name << "\n";
    if (this == &other) { // Handle self-assignment
        return *this;
    }
    name = other.name;
    damage = other.damage;
    reloadTime = other.reloadTime;
    range = other.range;
    return *this;
}

// --- Getters (all const) ---
float Weapon::getDamage() const {
    return damage;
}

float Weapon::getReloadTime() const {
    return reloadTime;
}

float Weapon::getRange() const {
    return range;
}

std::string Weapon::getName() const {
    return name;
}

float Weapon::attack() const {
    // In a real game, this might trigger an animation or cooldown
    return damage;
}

// --- operator<< ---
std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
    os << "Weapon(Name: " << weapon.name
       << ", Dmg: " << weapon.damage
       << ", Rng: " << weapon.range
       << ", Spd: " << weapon.reloadTime << ")";
    return os;
}