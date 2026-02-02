#ifndef WEAPON_H
#define WEAPON_H
#include <SFML/Graphics.hpp>

#include <string>

enum class WeaponType {
    Melee,
    Ranged,
    Magic,
    Throwable
};

class Weapon {
public:
    Weapon();
    Weapon(std::string name, float damage, float reloadTime, float range, WeaponType type = WeaponType::Ranged);

    void loadTexture(const std::string& path);
    void update(sf::Vector2f playerPos, sf::Angle rotationAngle);

    virtual void render(sf::RenderWindow& window) const;

    // 1. Destructor
    virtual ~Weapon();
    // 2. Copy Constructor
    Weapon(const Weapon& other);
    // 3. Copy Assignment Operator
    Weapon& operator=(const Weapon& other);
    // ---------------------

    float getDamage() const;
    float getReloadTime() const;
    WeaponType getType() const;
    float getRange() const;
    const std::string& getName() const;

    void setDamage(float newDamage);
    void setVisualSize(float width, float height);

    void setScale(float scale);

    virtual std::unique_ptr<Weapon> clone() const = 0;

    // float attack() const;

    // operator<< for display
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon);
    virtual void print(std::ostream& os) const;
    virtual void applyUpgrade() = 0;

protected: // Changed from protected for better encapsulation

    sf::Texture mTexture;
    sf::Sprite mSprite;

    WeaponType type;
    std::string name;
    float damage;
    float reloadTime;
    float range;
};


#endif //WEAPON_H