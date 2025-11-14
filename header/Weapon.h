#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <ostream> // For std::ostream

class Weapon {
public:
    Weapon();
    // Parameterized constructor
    Weapon(std::string name, float damage, float reloadTime, float range);

    // --- Rule of Three ---
    // 1. Destructor
    virtual ~Weapon();
    // 2. Copy Constructor
    Weapon(const Weapon& other);
    // 3. Copy Assignment Operator
    Weapon& operator=(const Weapon& other);
    // ---------------------

    float getDamage() const;
    float getReloadTime() const;
    float getRange() const;
    std::string getName() const; // Made const

    float attack() const; // Made const

    // operator<< for display
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon);

private: // Changed from protected for better encapsulation
    std::string name;
    float damage;
    float reloadTime;
    float range;
};

#endif //WEAPON_H