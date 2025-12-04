#include "../header/Weapon.h"
#include <iostream>

// 1. Default constructor
Weapon::Weapon()
    : name("Fists"),
      damage(1.f),
      reloadTime(0.5f),
      range(10.f),
      // Initialize visuals
      mTexture{},
      mSprite(mTexture)
{
    std::cout << "Weapon default constructor called.\n";
}

// 2. Parameterized constructor
Weapon::Weapon(std::string name, float damage, float reloadTime, float range)
    : name(std::move(name)),
      damage(damage),
      reloadTime(reloadTime),
      range(range),
      // Initialize visuals
      mTexture{},
      mSprite(mTexture)
{
    std::cout << "Weapon parameterized constructor called for: " << this->name << "\n";
}

// 3. Destructor
Weapon::~Weapon() {
    std::cout << "Weapon destructor called for: " << name << "\n";
}

// 4. Copy Constructor (CRITICAL FIX)
Weapon::Weapon(const Weapon& other)
    : name(other.name),
      damage(other.damage),
      reloadTime(other.reloadTime),
      range(other.range),
      // Copy the texture and sprite
      mTexture(other.mTexture),
      mSprite(other.mSprite)
{
    std::cout << "Weapon COPY CONSTRUCTOR called to copy: " << other.name << "\n";

    // IMPORTANT: After copying the texture, we must ensure the sprite uses THIS new texture,
    // not the one from the 'other' weapon.
    if (mTexture.getSize().x > 0) { // Only if texture was actually loaded
        mSprite.setTexture(mTexture);
    }
}

// 5. Copy Assignment Operator (CRITICAL FIX)
Weapon& Weapon::operator=(const Weapon& other) {
    std::cout << "Weapon COPY ASSIGNMENT called to copy: " << other.name << "\n";

    if (this == &other) { // Handle self-assignment
        return *this;
    }

    // Copy basic stats
    name = other.name;
    damage = other.damage;
    reloadTime = other.reloadTime;
    range = other.range;

    // Copy visuals
    mTexture = other.mTexture;
    mSprite = other.mSprite;

    // Re-link sprite to the new texture copy
    if (mTexture.getSize().x > 0) {
        mSprite.setTexture(mTexture);
    }

    return *this;
}


void Weapon::render(sf::RenderWindow& window) const {
    window.draw(mSprite);
}

void Weapon::loadTexture(const std::string& path) {
    if (mTexture.loadFromFile(path)) {
        mSprite.setTexture(mTexture);
        // Center the weapon so it rotates around the player's hand
        mSprite.setOrigin({static_cast<float>(mTexture.getSize().x) / 2.f, static_cast<float> (mTexture.getSize().y) / 2.f});
    }
}

void Weapon::update(sf::Vector2f playerPos, sf::Angle rotationAngle) {
    // Offset the weapon slightly so it looks like it's in hand
    mSprite.setPosition(playerPos);
    mSprite.setRotation(rotationAngle);
}



// --- Getters (all const) ---
float Weapon::getDamage() const {
    return damage;
}

float Weapon::getReloadTime() const {
    return reloadTime;
}

// float Weapon::getRange() const {
//     return range;
// }

// std::string Weapon::getName() const {
//     return name;
// }

// float Weapon::attack() const {
//     // In a real game, this might trigger an animation or cooldown
//     return damage;
// }

// --- operator<< ---
std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
    os << "Weapon(Name: " << weapon.name
       << ", Dmg: " << weapon.damage
       << ", Rng: " << weapon.range
       << ", Spd: " << weapon.reloadTime << ")";
    return os;
}