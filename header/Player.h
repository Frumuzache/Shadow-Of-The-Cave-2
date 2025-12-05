#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Weapon.h"

#pragma once

class Player : public Entity {
public:
    Player();

    void update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) const override;
    sf::Vector2f getPlayerPosition() const;

    static Player& getInstance();
    sf::Vector2u getTextureSize() const;

    Weapon& getWeapon();
    static float getAttackDamage() { return 10.f; } // Later, this comes from a Weapon
    static float getAttackRange()  { return 50.f; } // Later, this comes from a Weapon
    sf::Angle getRotation() const;


private:
    // These functions are specific to the Player's update
    void updateMovement([[maybe_unused]]sf::Time deltaTime, sf::Vector2f mapBounds);
    void updateRotation(const sf::RenderWindow& window);
    static void updateHealth([[maybe_unused]]sf::Time deltaTime);
    Weapon mWeapon;

};

#endif //PLAYER_H