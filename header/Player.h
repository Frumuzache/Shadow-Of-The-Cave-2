#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "RangedWeapon.h"
#include "MeleeWeapon.h"
#include "ThrowableWeapon.h"
#include "MagicWeapon.h"
#include <SFML/Graphics.hpp>

class Player : public Entity {
public:
    static Player& getInstance();

    // Deleted copy constructor/assignment for Singleton
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    void update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) const override;

    // Getters
    sf::Vector2f getPlayerPosition() const;
    sf::Vector2u getTextureSize() const;
    // sf::Angle getRotation() const;


    RangedWeapon& getRangedWeapon();
    MeleeWeapon& getMeleeWeapon();
    ThrowableWeapon& getThrowableWeapon();
    MagicWeapon& getMagicWeapon();


    Weapon& getActiveWeapon();

    void takeDamage(float damage) override;
    void heal(float amount);
    bool isDead() const;

private:
    Player(); // Private constructor

    void updateMovement(sf::Time deltaTime, sf::Vector2f mapBounds);
    void updateRotation(const sf::RenderWindow& window);
    static void updateHealth(sf::Time deltaTime);

    // --- Loading Functions ---
    void loadRangedWeapon();
    void loadMeleeWeapon();
    void loadThrowableWeapon();


    // Inventory
    RangedWeapon mRangedWeapon;
    MeleeWeapon mMeleeWeapon;
    ThrowableWeapon mThrowableWeapon;
    MagicWeapon mMagicWeapon;


    // Grenade assets must be owned by Player to be valid for mThrowableWeapon
    sf::Texture mGrenadeTexture;

};

#endif