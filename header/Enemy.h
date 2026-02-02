#pragma once
#include "Entity.h"
#include "MeleeWeapon.h"
#include "RangedWeapon.h"
#include <SFML/System/Clock.hpp>
#include <memory>

// Forward declaration to avoid circular dependency
class Game;

enum class EnemyType {
    Melee,   // Melee enemy with close-range weapon
    Ranged   // Ranged enemy with projectile-based weapon
};

class Enemy : public Entity {
public:
    // Constructors
    Enemy();
    Enemy(sf::Vector2f startPosition, float speed, float health, EnemyType type = EnemyType::Melee);

    // Public Methods
    void update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) override;
    void takeDamage(float damageAmount);
    static void death();
    void render(sf::RenderWindow &window) const override;

    // Getter for enemy type
    EnemyType getEnemyType() const;

    // Set reference to Game (for ranged enemies to fire projectiles)
    void setGameReference(Game* game) { mGame = game; }

    // Debugging
    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy);

protected:
    // Protected Attributes (accessible by derived classes like RangedEnemy)
    std::unique_ptr<Weapon> mWeapon;    // Polymorphic weapon (Melee or Ranged)
    EnemyType mEnemyType;

private:
    // Private Helper Methods
    void updateMovementEnemy(sf::Time deltaTime, const sf::Vector2f& mapBounds);
    void loadAssets();
    void tryAttack(); // Logic to check range and deal damage
    void tryProjectileAttack(); // For ranged enemies
    void fireProjectile(); // Helper to fire projectile towards player
    void updateWeaponRotation(); // Update weapon rotation towards player (for ranged enemies)

    sf::RectangleShape mHealthBarBackground;
    sf::RectangleShape mHealthBarForeground;

    // Helper to keep code clean
    void updateHealthBarVisuals();

    // Private Attributes
    sf::Vector2f mDirection;
    sf::Vector2f initialPosition;
    Game* mGame; // Reference to game for firing projectiles

    // Combat Attributes (Private)
    sf::Clock mAttackClock; // Tracks time since last attack
};