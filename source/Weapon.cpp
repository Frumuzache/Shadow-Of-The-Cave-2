#include "../header/Weapon.h"
#include <iostream>

// 1. Default constructor
Weapon::Weapon()
    : name("Fists"),
      damage(1.f),
      reloadTime(0.5f),
      range(10.f),
      type(WeaponType::Melee),
      // Initialize visuals
      mTexture{},
      mSprite(mTexture)
{
    std::cout << "Weapon default constructor called.\n";
}

// 2. Parameterized constructor
Weapon::Weapon(std::string name, float damage, float reloadTime, float range, WeaponType type)
    : name(std::move(name)),
      damage(damage),
      reloadTime(reloadTime),
      range(range),
      type(type),
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
      type(other.type),
      // Copy the texture and sprite
      mTexture(other.mTexture),
      mSprite(other.mSprite)
{
    std::cout << "Weapon COPY CONSTRUCTOR called to copy: " << other.name << "\n";

    // IMPORTANT: After copying the texture, we must ensure the sprite uses THIS new texture,
    // not the one from the 'other' weapon.
    if (mTexture.getSize().x > 0) { // Only if texture was actually loaded
        mSprite.setTexture(mTexture, true);
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
    type = other.type;
    // Copy visuals
    mTexture = other.mTexture;
    mSprite = other.mSprite;

    // Re-link sprite to the new texture copy
    if (mTexture.getSize().x > 0) {
        mSprite.setTexture(mTexture, true);
    }

    return *this;
}


void Weapon::render(sf::RenderWindow& window) const {
    if (mTexture.getSize().x > 0) {
        window.draw(mSprite);
    }
    else {
        // FALLBACK: Draw a Red Rectangle so we can see the weapon exists
        sf::RectangleShape debugShape({40.f, 6.f}); // Long red stick
        debugShape.setOrigin({0.f, 3.f});           // Center it
        debugShape.setFillColor(sf::Color::Red);

        // Match the weapon's position and rotation
        debugShape.setPosition(mSprite.getPosition());
        debugShape.setRotation(mSprite.getRotation());

        window.draw(debugShape);
    }
}

void Weapon::loadTexture(const std::string& path) {
    if (mTexture.loadFromFile(path)) {
        mSprite.setTexture(mTexture, true);
        // Center the weapon
        mSprite.setOrigin({
            static_cast<float>(mTexture.getSize().x) / 2.f,
            static_cast<float>(mTexture.getSize().y) / 2.f
        });
        std::cout << "SUCCESS: Weapon texture loaded from " << path << "\n";
    } else {
        // ERROR MESSAGE
        std::cerr << "ERROR: Could not load weapon texture: " << path << "\n";
        std::cerr << "      (Ensure the file exists in the 'assets' folder)\n";
    }
}

void Weapon::update(sf::Vector2f playerPos, sf::Angle rotationAngle) {
    // Offset the weapon slightly so it looks like it's in hand
    mSprite.setPosition(playerPos);
    mSprite.setRotation(rotationAngle);
}




WeaponType Weapon::getType() const {
    return type;
}

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


void Weapon::setScale(float scale) {
    mSprite.setScale({scale, scale});
}

void Weapon::setVisualSize(float width, float height) {
    // Safety check to avoid dividing by zero if texture isn't loaded yet
    sf::Vector2u texSize = mTexture.getSize();
    if (texSize.x == 0 || texSize.y == 0) return;

    // Calculate how much we need to shrink/grow the texture to fit the target size
    float scaleX = width / static_cast<float>(texSize.x);
    float scaleY = height / static_cast<float>(texSize.y);

    mSprite.setScale({scaleX, scaleY});
}



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