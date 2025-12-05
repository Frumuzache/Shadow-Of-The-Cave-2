#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <ostream> // For std::ostream
#include "Enemy.h"
#include "Player.h"
#include "Level.h"
#include "HUD.h"
#include "Projectile.h"

class Game {
public:
    // Parameterized constructor
    Game(unsigned int width, unsigned int height, const std::string& title);

    void run();

    // sf::Vector2u getWindowSize() const; // Made const

    // operator<< for display
    friend std::ostream& operator<<(std::ostream& os, const Game& game);

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void resolveEnemyCollisions() const;

    void handleInput();
    void handleShooting();
    void handleMeleeAttack();
    void updateCamera();
    void updateProjectiles(sf::Time deltaTime);
    void checkCollisions();
    void cleanupEntities();
    bool mIsGameOver;

    sf::Clock mSurvivalClock;    // Cât timp a rezistat jucătorul (pentru dificultate)
    float mSpawnTimer;           // Cât timp a trecut de la ultimul spawn
    float mSpawnInterval;        // Cât așteptăm până la următorul spawn (se va micșora)

    void handleEnemySpawning(sf::Time deltaTime); // Funcție nouă helper
    void spawnOneEnemy();


    // Composition: Game "has-a" window, clock, level, hud, player, and enemies
    sf::RenderWindow mWindow;
    sf::Clock mClock;

    sf::View mView;
    sf::Vector2f mWorldSize;

    Level mLevel; // Replaces background texture/sprite
    HUD mHUD;     // New composed object

    Player& mPlayer; // Get the singleton instance
    std::vector<std::unique_ptr<Enemy>> mEnemies;
    std::vector<Projectile> mProjectiles;
};

#endif //GAME_H