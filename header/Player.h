/**
 * @file Player.h
 * @brief Player character class
 *
 * Implements the player character with singleton pattern.
 * Manages player movement, weapons, health, and rendering.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "RangedWeapon.h"
#include "MeleeWeapon.h"
#include "ThrowableWeapon.h"
#include "MagicWeapon.h"
#include <SFML/Graphics.hpp>

/**
 * @class Player
 * @brief Player character (Singleton)
 *
 * Represents the player in the game. Implemented as a singleton to ensure
 * only one player instance exists. Inherits from Entity and manages:
 * - Movement and rotation towards mouse
 * - Multiple weapons (ranged, melee, throwable, magic)
 * - Health and taking damage
 * - Rendering and animation
 *
 * @note Uses singleton pattern - access via getInstance()
 */
class Player : public Entity {
public:
    /**
     * @brief Get the singleton instance of Player
     * @return Reference to the unique Player instance
     */
    static Player& getInstance();

    /// @brief Deleted copy constructor for Singleton pattern
    Player(const Player&) = delete;

    /// @brief Deleted assignment operator for Singleton pattern
    Player& operator=(const Player&) = delete;

    /**
     * @brief Update player state each frame
     * @param deltaTime Time elapsed since last frame
     * @param mapBounds Game world boundaries for movement clamping
     * @param window Reference to render window for input and rotation
     */
    void update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) override;

    /**
     * @brief Render player sprite to window
     * @param window Target render window
     */
    void render(sf::RenderWindow& window) const override;

    /// @name Position and Size Queries
    /// @{
    /**
     * @brief Get player's current position
     * @return Vector2f with x, y coordinates
     */
    sf::Vector2f getPlayerPosition() const;

    /**
     * @brief Get player's texture size
     * @return Vector2u with width and height in pixels
     */
    sf::Vector2u getTextureSize() const;
    /// @}

    /// @name Weapon Access
    /// @{
    /**
     * @brief Get reference to ranged weapon
     * @return Reference to RangedWeapon (rifle)
     */
    RangedWeapon& getRangedWeapon();

    /**
     * @brief Get reference to melee weapon
     * @return Reference to MeleeWeapon (sword)
     */
    MeleeWeapon& getMeleeWeapon();

    /**
     * @brief Get reference to throwable weapon
     * @return Reference to ThrowableWeapon (grenades)
     */
    ThrowableWeapon& getThrowableWeapon();

    /**
     * @brief Get reference to magic weapon
     * @return Reference to MagicWeapon
     */
    MagicWeapon& getMagicWeapon();

    /**
     * @brief Get the currently active weapon
     * @return Reference to the currently equipped Weapon
     */
    Weapon& getActiveWeapon();
    /// @}

    /// @name Health Management
    /// @{
    /**
     * @brief Apply damage to player
     * @param damage Amount of damage to apply
     */
    void takeDamage(float damage) override;

    /**
     * @brief Heal the player
     * @param amount Amount of health to restore
     */
    void heal(float amount);

    /**
     * @brief Check if player is dead
     * @return true if current health <= 0, false otherwise
     */
    bool isDead() const;
    /// @}

private:
    /// @brief Private constructor for Singleton pattern
    Player();

    /// @name Private Update Methods
    /// @{
    /**
     * @brief Update player movement based on keyboard input
     * @param deltaTime Time elapsed since last frame
     * @param mapBounds Game world boundaries
     */
    void updateMovement(sf::Time deltaTime, sf::Vector2f mapBounds);

    /**
     * @brief Update player rotation towards mouse cursor
     * @param window Reference to render window for mouse position
     */
    void updateRotation(const sf::RenderWindow& window);

    /**
     * @brief Update health status (static method)
     * @param deltaTime Time elapsed since last frame
     */
    static void updateHealth(sf::Time deltaTime);
    /// @}

    /// @name Weapon Loading
    /// @{
    /// @brief Load and initialize ranged weapon
    void loadRangedWeapon();

    /// @brief Load and initialize melee weapon
    void loadMeleeWeapon();

    /// @brief Load and initialize throwable weapon
    void loadThrowableWeapon();
    /// @}

    /// @name Weapon Inventory
    /// @{
    RangedWeapon mRangedWeapon;      ///< Rifle weapon
    MeleeWeapon mMeleeWeapon;        ///< Sword weapon
    ThrowableWeapon mThrowableWeapon;  ///< Grenade weapon
    MagicWeapon mMagicWeapon;        ///< Magic spell weapon
    /// @}

    /// @brief Grenade texture (owned by Player for validity)
    sf::Texture mGrenadeTexture;

};

#endif