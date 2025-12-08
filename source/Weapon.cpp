#include "../header/Weapon.h"
#include <iostream>
#include "../header/GameException.h"

// Constructors
Weapon::Weapon()
    : Weapon("Fists", 1.f, 0.5f, 10.f, WeaponType::Melee)
{
    std::cout << "Weapon default constructor called.\n";
}

Weapon::Weapon(std::string name, float damage, float reloadTime, const float range, WeaponType type)
    : mTexture{}, mSprite(mTexture), type(type), name(std::move(name)), damage(damage),
      reloadTime(reloadTime), range(range)
{
    std::cout << "Weapon created: " << this->name << "\n";
    if (damage < 0.f) {
        throw InvalidStatException(this->name, "Damage", damage);
    }
    if (range <= 0.f) {
        throw InvalidStatException(this->name, "Range", range);
    }
    if (reloadTime < 0.f) {
        throw InvalidStatException(this->name, "ReloadTime", reloadTime);
    }
}

Weapon::~Weapon() {
    std::cout << "Weapon destroyed: " << name << "\n";
}

// Rule of Three (Copying logic)
Weapon::Weapon(const Weapon& other)
    : mTexture(other.mTexture), mSprite(other.mSprite), type(other.type), name(other.name), damage(other.damage),
      reloadTime(other.reloadTime), range(other.range)
{
    if (mTexture.getSize().x > 0) mSprite.setTexture(mTexture, true);
}

Weapon& Weapon::operator=(const Weapon& other) {
    if (this == &other) return *this;

    name = other.name;
    damage = other.damage;
    reloadTime = other.reloadTime;
    range = other.range;
    type = other.type;

    mTexture = other.mTexture;
    mSprite = other.mSprite;

    if (mTexture.getSize().x > 0) mSprite.setTexture(mTexture, true);
    return *this;
}

// Visual Logic
void Weapon::loadTexture(const std::string& path) {
    if (!mTexture.loadFromFile(path)) {
        throw AssetLoadException("Weapon Texture (" + name + ")", path);
    }
    mSprite.setTexture(mTexture, true);
    mSprite.setOrigin({
        static_cast<float>(mTexture.getSize().x) / 2.f,
        static_cast<float>(mTexture.getSize().y) / 2.f});
}

void Weapon::setVisualSize(float width, float height) {
    sf::Vector2u texSize = mTexture.getSize();
    if (texSize.x == 0 || texSize.y == 0) return;

    float scaleX = width / static_cast<float>(texSize.x);
    float scaleY = height / static_cast<float>(texSize.y);
    mSprite.setScale({scaleX, scaleY});
}

void Weapon::update(sf::Vector2f playerPos, sf::Angle rotationAngle) {
    mSprite.setPosition(playerPos);
    mSprite.setRotation(rotationAngle);
}

void Weapon::render(sf::RenderWindow& window) const {
    if (mTexture.getSize().x > 0) {
        window.draw(mSprite);
    } else {
        // Debug Red Box if texture fails
        sf::RectangleShape debugShape({40.f, 6.f});
        debugShape.setOrigin({0.f, 3.f});
        debugShape.setFillColor(sf::Color::Red);
        debugShape.setPosition(mSprite.getPosition());
        debugShape.setRotation(mSprite.getRotation());
        window.draw(debugShape);
    }
}

// Getters
WeaponType Weapon::getType() const { return type; }
float Weapon::getDamage() const { return damage; }
float Weapon::getReloadTime() const { return reloadTime; }
float Weapon::getRange() const { return range; }
const std::string& Weapon::getName() const { return name; }
void Weapon::setScale(float scale) { mSprite.setScale({scale, scale}); }







std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
    weapon.print(os); // Delegates to the virtual function
    return os;
}


// --- VIRTUAL PRINT IMPLEMENTATION (Base Behavior) ---
void Weapon::print(std::ostream& os) const {
    os << "Weapon: " << name << " | Dmg: " << damage << " | Rng: " << range;
}