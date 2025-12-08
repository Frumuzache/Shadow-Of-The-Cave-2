#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Player.h"
#include "HUD.h"
#include "Level.h"
#include "Enemy.h"
#include "Projectile.h"
#include "ThrowableWeapon.h"

class Game {
public:
    Game(unsigned int width, unsigned int height, const std::string& title);
    void run();

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

    // Grenade Update Logic
    void updateGrenades(sf::Time deltaTime);

    void handleEnemySpawning(sf::Time deltaTime);
    void spawnOneEnemy();
    void checkCollisions();
    void resolveEnemyCollisions() const;
    void cleanupEntities();

    // Member Variables
    sf::RenderWindow mWindow;
    sf::View mView;
    sf::Vector2f mWorldSize;

    Level mLevel;
    HUD mHUD;
    Player& mPlayer;

    std::vector<std::unique_ptr<Entity>> mEnemies;
    std::vector<Projectile> mProjectiles;

    // --- Added missing variables to fix Game.cpp errors ---
    std::vector<std::unique_ptr<ThrowableWeapon>> mActiveGrenades;
    sf::Clock mGrenadeCooldown;
    sf::Texture mGrenadeTexture;

    sf::Clock mClock;
    sf::Clock mSurvivalClock;
    float mSpawnTimer;
    float mSpawnInterval;
    bool mIsGameOver;
    sf::Clock mUpgradeClock;
};

std::ostream& operator<<(std::ostream& os, const Game& game);

#endif