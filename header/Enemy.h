/**
 * @file Enemy.h
 * @brief Base enemy class for AI controlled entities
 */

#pragma once
#include "Entity.h"
#include "Weapon.h"
#include <SFML/System/Clock.hpp>
#include <memory>

/// Forward declaration to avoid circular dependency
class Game;

/**
 * @enum EnemyType
 * @brief Types of enemies in the game
 */
enum class EnemyType {
    Melee,   ///< Melee enemy with close-range weapon
    Ranged   ///< Ranged enemy with projectile-based weapon
};

/**
 * @class Enemy
 * @brief Base class for all AI-controlled enemies
 *
 * Manages:
 * - Enemy AI and movement
 * - Weapon management and attacks
 * - Health and collision with player
 * - Rendering with health bar display
 *
 * Derived classes: MeleeEnemy, RangedEnemy
 *
 * @see MeleeEnemy, RangedEnemy, EnemyType
 */
class Enemy : public Entity {
public:
    /// @brief Default constructor
    Enemy();

    /**
     * @brief Parameterized constructor
     * @param startPosition Starting position in world
     * @param speed Movement speed
     * @param health Maximum health
     * @param type Enemy type (default: Melee)
     */
    Enemy(sf::Vector2f startPosition, float speed, float health, EnemyType type = EnemyType::Melee);

    /// @name Core Methods
    /// @{
    /**
     * @brief Update enemy state each frame
     * @param deltaTime Time elapsed since last frame
     * @param mapBounds Game world boundaries
     * @param window Reference to render window
     */
    void update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) override;

    /**
     * @brief Apply damage to enemy
     * @param damageAmount Amount of damage to apply
     */
    void takeDamage(float damageAmount) override;

    /// @brief Called when enemy dies
    static void death();

    /**
     * @brief Render enemy sprite and health bar
     * @param window Target render window
     */
    void render(sf::RenderWindow &window) const override;
    /// @}

    /// @name Property Getters
    /// @{
    /// @brief Get the type of this enemy
    EnemyType getEnemyType() const;
    /// @}

    /// @name Game Reference
    /// @{
    /// @brief Set reference to Game for projectile firing
    void setGameReference(Game* game) { mGame = game; }
    /// @}

    /// @brief Output stream operator for debugging
    friend std::ostream& operator<<(std::ostream& os, const Enemy& enemy);

protected:
    /// @name Protected Members - Combat
    /// @{
    std::unique_ptr<Weapon> mWeapon;    ///< Polymorphic weapon (Melee or Ranged)
    /// @}

    /// @name Protected Members - State
    /// @{
    EnemyType mEnemyType;               ///< Type of enemy
    sf::Vector2f mDirection;            ///< Current movement direction
    sf::Vector2f initialPosition;       ///< Starting position
    Game* mGame;                        ///< Reference to game for firing projectiles
    /// @}

private:
    /// @name Private Helper Methods
    /// @{
    /**
     * @brief Update enemy movement towards player
     * @param deltaTime Time elapsed since last frame
     * @param mapBounds Game world boundaries
     */
    void updateMovementEnemy(sf::Time deltaTime, const sf::Vector2f& mapBounds);

    /// @brief Load enemy sprite assets
    void loadAssets();

    /// @brief Attempt to attack player if in range
    void tryAttack();

    /// @brief Fire projectile (for ranged enemies)
    void fireProjectile();

    /// @brief Update weapon rotation towards player
    void updateWeaponRotation();

    /**
     * @brief Update health bar visual representation
     */
    void updateHealthBarVisuals();
    /// @}

    /// @name Private Members - Rendering
    /// @{
    sf::RectangleShape mHealthBarBackground;  ///< Health bar background shape
    sf::RectangleShape mHealthBarForeground;  ///< Health bar fill shape
    /// @}

    /// @name Private Members - Combat
    /// @{
    sf::Clock mAttackClock;             ///< Tracks time since last attack
    /// @}
};