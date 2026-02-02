/**
 * @file Game.h
 * @brief Main game controller class
 *
 * Manages the main game loop, rendering, updates, and game state.
 * Implements the game engine using SFML graphics library.
 * Uses object pooling for projectiles to optimize performance.
 */

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Player.h"
#include "HUD.h"
#include "Level.h"
#include "Enemy.h"
#include "MeleeEnemy.h"
#include "RangedEnemy.h"
#include "Projectile.h"
#include "EnemyProjectile.h"
#include "ThrowableWeapon.h"
#include "Inventory.h"
#include "Pool.h"

/**
 * @class Game
 * @brief Main game engine controller
 *
 * Manages:
 * - Game loop (event processing, updating, rendering)
 * - Player and enemy management
 * - Collision detection and resolution
 * - Projectile pooling
 * - Shop system
 * - HUD and camera updates
 *
 * @see Player, Enemy, Level, HUD
 */
class Game {
public:
    /**
     * @brief Constructor for Game
     * @param width Window width in pixels
     * @param height Window height in pixels
     * @param title Window title string
     * @throws GameConfigException if width or height is invalid
     */
    Game(unsigned int width, unsigned int height, const std::string& title);

    /**
     * @brief Main game loop
     *
     * Continuously processes events, updates game state, and renders.
     * Runs until window is closed or game over condition is reached.
     */
    void run();

    /**
     * @brief Add an enemy projectile to the game
     * @param projectile The EnemyProjectile to add
     */
    void addEnemyProjectile(const EnemyProjectile& projectile) {
        mEnemyProjectiles.push_back(projectile);
    }

private:
    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    /// @name Event and Input Handling
    /// @{
    /// @brief Process window events (close, resize, etc.)
    void processEvents();

    /// @brief Handle keyboard input for player movement and actions
    void handleInput();

    /// @brief Handle player shooting with ranged weapon
    void handleShooting();

    /// @brief Handle player melee attack
    void handleMeleeAttack() const;

    /// @brief Handle grenade throw logic
    void handleGrenadeThrow();

    /// @brief Handle shop input and purchasing
    void handleShopInput();
    /// @}

    /// @name Game Loop Methods
    /// @{
    /// @brief Update game state for current frame
    /// @param deltaTime Time elapsed since last frame
    void update(sf::Time deltaTime);

    /// @brief Render all game objects
    void render();
    /// @}

    /// @name Camera Management
    /// @{
    /// @brief Update camera position to follow player
    void updateCamera();
    /// @}

    /// @name Projectile Management
    /// @{
    /// @brief Update player projectiles
    /// @param deltaTime Time elapsed since last frame
    void updateProjectiles(sf::Time deltaTime);

    /// @brief Update enemy projectiles
    /// @param deltaTime Time elapsed since last frame
    void updateEnemyProjectiles(sf::Time deltaTime);

    /// @brief Update grenade state and explosions
    /// @param deltaTime Time elapsed since last frame
    void updateGrenades(sf::Time deltaTime);
    /// @}

    /// @name Enemy Management
    /// @{
    /// @brief Handle enemy spawning logic
    /// @param deltaTime Time elapsed since last frame
    void handleEnemySpawning(sf::Time deltaTime);

    /// @brief Spawn a single enemy at random location
    void spawnOneEnemy();
    /// @}

    /// @name Collision and Cleanup
    /// @{
    /// @brief Check all collision interactions in the game
    void checkCollisions();

    /// @brief Resolve enemy-to-enemy collisions
    void resolveEnemyCollisions() const;

    /// @brief Remove dead entities and finished projectiles
    void cleanupEntities();
    /// @}

    /// @name Shop System Methods
    /// @{
    /// @brief Purchase a grenade
    void buyGrenade();

    /// @brief Purchase a health potion
    void buyHealthPotion();

    /// @brief Purchase a damage boost
    void buyDamageBoost();

    /// @brief Purchase magic damage unlock
    void buyMagicDamage();

    /// @brief Use a health potion from inventory
    void useHealthPotion();

    /// @brief Use a grenade from inventory
    void useGrenade();

    /// @brief Use magic damage ability
    void useMagicDamage();
    /// @}

    /// @name Member Variables - Rendering
    /// @{
    sf::RenderWindow mWindow;              ///< SFML render window
    sf::View mView;                        ///< Camera view for following player
    sf::Vector2f mWorldSize;               ///< Game world dimensions
    /// @}

    /// @name Member Variables - Game Objects
    /// @{
    Level mLevel;                          ///< Game level/background
    HUD mHUD;                              ///< Head-up display
    Player& mPlayer;                       ///< Reference to player (singleton)

    std::vector<std::unique_ptr<Entity>> mEnemies;  ///< Active enemies in game
    std::vector<Projectile> mProjectiles;           ///< Player projectiles
    std::vector<EnemyProjectile> mEnemyProjectiles; ///< Enemy projectiles

    std::vector<std::unique_ptr<ThrowableWeapon>> mActiveGrenades; ///< Active grenades
    sf::Clock mGrenadeCooldown;            ///< Grenade throw cooldown timer
    sf::Texture mGrenadeTexture;           ///< Grenade sprite texture
    /// @}

    /// @name Member Variables - Timers
    /// @{
    sf::Clock mClock;                      ///< Main game clock
    sf::Clock mSurvivalClock;              ///< Survival time counter
    sf::Clock mUpgradeClock;               ///< Upgrade timer
    float mSpawnTimer;                     ///< Current spawn timer value
    float mSpawnInterval;                  ///< Time between enemy spawns
    bool mIsGameOver;                      ///< Game over flag
    /// @}

    /// @name Member Variables - Object Pooling
    /// @{
    Pool<Projectile> mProjectilePool;      ///< Projectile object pool
    Pool<EnemyProjectile> mEnemyProjectilePool; ///< Enemy projectile object pool
    /// @}

    /// @name Member Variables - Game State
    /// @{
    int mCoins = 0;                        ///< Player coins (Studenți Salvați)
    Inventory mInventory;                  ///< Player inventory for consumables
    bool mMagicUnlocked = false;           ///< Whether magic weapon is unlocked
    /// @}

    /// @name Member Variables - Shop Pricing
    /// @{
    const int GRENADE_COST = 50;           ///< Cost of one grenade
    const int HEALTH_POTION_COST = 25;     ///< Cost of health potion
    const int DAMAGE_BOOST_COST = 40;      ///< Cost of damage boost
    const int MAGIC_DAMAGE_COST = 75;      ///< Cost of magic unlock
    const float DAMAGE_BOOST_MULTIPLIER = 1.10f;  ///< 10% damage multiplier
    /// @}
};

std::ostream& operator<<(std::ostream& os, const Game& game);

#endif