#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Weapon.h"

#pragma once

class Player : public Entity {
public:
    Player();

    // "override" tells the compiler we are implementing
    // the pure virtual function from our parent (Entity)
    void update(sf::Time deltaTime, const sf::Window& window) override;

    sf::Vector2f getPlayerPosition() const;

    static Player& getInstance();
    sf::Vector2u getTextureSize() const;

    Weapon& getWeapon();

    static float getAttackDamage() { return 10.f; } // Later, this comes from a Weapon
    static float getAttackRange()  { return 50.f; } // Later, this comes from a Weapon
    ;


private:
    // These functions are specific to the Player's update
    void updateMovement([[maybe_unused]]sf::Time deltaTime, const sf::Window& window);
    void updateHealth([[maybe_unused]]sf::Time deltaTime);
    Weapon mWeapon;

};

#endif //PLAYER_H