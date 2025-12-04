#ifndef WEAPON_H
#define WEAPON_H
#include <SFML/Graphics.hpp>

#include <string>
#include <ostream> // For std::ostream

class Weapon {
public:
    Weapon();
    Weapon(std::string name, float damage, float reloadTime, float range);

    void loadTexture(const std::string& path);
    void update(sf::Vector2f playerPos, sf::Angle rotationAngle);
    void render(sf::RenderWindow& window) const;

    // 1. Destructor
    virtual ~Weapon();
    // 2. Copy Constructor
    Weapon(const Weapon& other);
    // 3. Copy Assignment Operator
    Weapon& operator=(const Weapon& other);
    // ---------------------

    float getDamage() const;
    float getReloadTime() const;
    // float getRange() const;
    // std::string getName() const;
    //
    // float attack() const;

    // operator<< for display
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon);

private: // Changed from protected for better encapsulation

    sf::Texture mTexture;
    sf::Sprite mSprite;

    std::string name;
    float damage;
    float reloadTime;
    float range;
};

#endif //WEAPON_H