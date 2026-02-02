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

class Game {
public:
    Game(unsigned int width, unsigned int height, const std::string& title);
    void run();

    // Public method for enemies to fire projectiles
    void addEnemyProjectile(const EnemyProjectile& projectile) {
        mEnemyProjectiles.push_back(projectile);
    }

private:
    friend std::ostream& operator<<(std::ostream& os, const Game& game);

    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    void handleInput();
    void handleShooting();
    void handleMeleeAttack() const;

    // Grenade Input Handler
    void handleGrenadeThrow();

    void updateCamera();
    void updateProjectiles(sf::Time deltaTime);
    void updateEnemyProjectiles(sf::Time deltaTime);

    // Grenade Update Logic
    void updateGrenades(sf::Time deltaTime);

    void handleEnemySpawning(sf::Time deltaTime);
    void spawnOneEnemy();
    void checkCollisions();
    void resolveEnemyCollisions() const;
    void cleanupEntities();

    // Shop system methods
    void handleShopInput();
    void buyGrenade();
    void buyHealthPotion();
    void buyDamageBoost();
    void buyMagicDamage();
    void useHealthPotion();
    void useGrenade();
    void useMagicDamage();

    // Member Variables
    sf::RenderWindow mWindow;
    sf::View mView;
    sf::Vector2f mWorldSize;

    Level mLevel;
    HUD mHUD;
    Player& mPlayer;

    std::vector<std::unique_ptr<Entity>> mEnemies;
    std::vector<Projectile> mProjectiles;
    std::vector<EnemyProjectile> mEnemyProjectiles;

    std::vector<std::unique_ptr<ThrowableWeapon>> mActiveGrenades;
    sf::Clock mGrenadeCooldown;
    sf::Texture mGrenadeTexture;

    sf::Clock mClock;
    sf::Clock mSurvivalClock;
    sf::Clock mUpgradeClock;
    float mSpawnTimer;
    float mSpawnInterval;
    bool mIsGameOver;

    // ✨ TEMPLATE CLASS INSTANTIATION 1: Pool<Projectile>
    // ✨ TEMPLATE CLASS INSTANTIATION 2: Pool<EnemyProjectile>
    Pool<Projectile> mProjectilePool;
    Pool<EnemyProjectile> mEnemyProjectilePool;

    // Coins system (Studenți Salvați)
    int mCoins = 0;

    // Inventory
    Inventory mInventory;

    bool mMagicUnlocked = false;

    // Shop pricing
    const int GRENADE_COST = 50;
    const int HEALTH_POTION_COST = 25;
    const int DAMAGE_BOOST_COST = 40;
    const int MAGIC_DAMAGE_COST = 75;
    const float DAMAGE_BOOST_MULTIPLIER = 1.10f;  // 10% boost
};

std::ostream& operator<<(std::ostream& os, const Game& game);

#endif