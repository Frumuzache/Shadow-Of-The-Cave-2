/**
 * @file Game.cpp
 * @brief Implementation of the Game class
 *
 * Implements the main game engine including:
 * - Game loop and rendering
 * - Collision detection
 * - Enemy spawning and management
 * - Projectile management with object pooling
 * - Shop system
 * - HUD updates
 *
 * @see Game.h
 */

#include <SFML/Graphics.hpp>
#include "../header/Game.h"
#include "../header/Player.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <algorithm>
#include <random>
#include "../header/GameException.h"

Game::Game(unsigned int width, unsigned int height, const std::string& title)
    : mWindow(sf::VideoMode({width, height}), title),
      mLevel("../assets/background.png", sf::Vector2f(3000.f, 3000.f)),
      mHUD("../assets/arial.ttf"),
      mPlayer(Player::getInstance()),
      mGrenadeCooldown{},
      mGrenadeTexture(),
      mClock{},
      mSurvivalClock{},
      mUpgradeClock{},
      mSpawnTimer(0.f),
      mSpawnInterval(3.0f),
      mIsGameOver(false),
      // ✨ TEMPLATE CLASS INSTANTIATION 1 & 2 INITIALIZATION
      mProjectilePool(20),           // Pre-allocate 20 player projectiles
      mEnemyProjectilePool(30)       // Pre-allocate 30 enemy projectiles
{
    std::cout << "\n=== TEMPLATE CLASSES INITIALIZED ===\n";
    std::cout << "Projectile Pool: " << mProjectilePool.getAvailableCount() << " objects\n";
    std::cout << "EnemyProjectile Pool: " << mEnemyProjectilePool.getAvailableCount() << " objects\n";
    std::cout << "====================================\n\n";
    if (width == 0 || height == 0) {
        throw GameConfigException("Window", "Dimensions cannot be zero.");
    }

    mWindow.setFramerateLimit(120);

    // Initialize first Enemies (mix of Melee and Ranged)
    auto melee1 = std::make_unique<MeleeEnemy>(sf::Vector2f(300.f, 100.f));
    melee1->setGameReference(this);
    mEnemies.push_back(std::move(melee1));

    // Spawn ranged enemy far away (corner of map) - distance > 5500px from player start
    auto ranged1 = std::make_unique<RangedEnemy>(sf::Vector2f(2950.f, 2950.f));
    ranged1->setGameReference(this);
    mEnemies.push_back(std::move(ranged1));

    auto melee2 = std::make_unique<MeleeEnemy>(sf::Vector2f(900.f, 700.f));
    melee2->setGameReference(this);
    mEnemies.push_back(std::move(melee2));

    // Initialize World & View
    mWorldSize = sf::Vector2f(3000.f, 3000.f);
    mView.setSize({static_cast<float>(width), static_cast<float>(height)});
    mView.setCenter({static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f});


    std::cout << "Game created successfully.\n";
}

void Game::run() {
    while (mWindow.isOpen()) {
        sf::Time deltaTime = mClock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    mWindow.setView(mView);

    while (const std::optional event = mWindow.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            mWindow.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                mWindow.close();
        }
    }

    if (!mIsGameOver) {
        handleInput();
    }
}

void Game::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        handleMeleeAttack();
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        handleShooting();
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
        handleGrenadeThrow();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) {
        if (mMagicUnlocked) {
            mPlayer.getMagicWeapon().castSpell();
        } else {
            std::cout << "Magic weapon not purchased yet!\n";
        }
    }

    // Handle shop inputs
    handleShopInput();
}

void Game::handleShooting() {
    static sf::Clock fireClock;

    const Weapon& currentWeapon = mPlayer.getRangedWeapon();

    if (fireClock.getElapsedTime().asSeconds() < currentWeapon.getReloadTime()) return;

    fireClock.restart();

    sf::Vector2f playerPos = mPlayer.getPlayerPosition();
    sf::Vector2u playerSize = mPlayer.getTextureSize();
    sf::Vector2f playerCenter = {
        playerPos.x + static_cast<float>(playerSize.x) / 2.f,
        playerPos.y + static_cast<float>(playerSize.y) / 2.f
    };

    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
    sf::Vector2f mouseWorld = mWindow.mapPixelToCoords(mousePos);
    sf::Vector2f directionVec = mouseWorld - playerCenter;

    float len = std::sqrt(directionVec.x * directionVec.x + directionVec.y * directionVec.y);
    sf::Vector2f normalizedDir = (len != 0) ? (directionVec / len) : sf::Vector2f(1, 0);

    mProjectiles.emplace_back(playerCenter, normalizedDir, 1000.f, currentWeapon.getDamage());
}

void Game::handleMeleeAttack() const {
    static sf::Clock meleeClock;

    const Weapon& currentWeapon = mPlayer.getMeleeWeapon();

    if (meleeClock.getElapsedTime().asSeconds() < currentWeapon.getReloadTime()) return;
    meleeClock.restart();

    sf::Vector2f playerPos = mPlayer.getPlayerPosition();
    sf::Vector2u pSize = mPlayer.getTextureSize();
    sf::Vector2f playerCenter = { playerPos.x + static_cast<float>(pSize.x) / 2.f, playerPos.y + static_cast<float>(pSize.y) / 2.f };

    const sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
    const sf::Vector2f mouseWorld = mWindow.mapPixelToCoords(mousePos);
    sf::Vector2f aimDir = mouseWorld - playerCenter;
    float len = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);
    if (len != 0) aimDir /= len;

    for (const auto& enemyPtr : mEnemies) {
        if (enemyPtr->getCurrentHealth() <= 0) continue;



        if (auto* enemy = dynamic_cast<Enemy*>(enemyPtr.get())) {
            if (enemy->getCurrentHealth() <= 0) continue;

            sf::Vector2f diff = enemy->getPosition() - playerCenter; // simplified for brevity
            float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

            if (dist <= currentWeapon.getRange()) {
                enemy->takeDamage(currentWeapon.getDamage());
                std::cout << "SLASH! Hit enemy.\n";
            }
        }


    }
}

void Game::handleGrenadeThrow() {
    if (mGrenadeCooldown.getElapsedTime().asSeconds() < 2.0f) return;

    // Check if player has grenades in inventory
    if (!mInventory.useGrenade()) {
        std::cout << "No grenades in inventory!\n";
        return;
    }

    mGrenadeCooldown.restart();

    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
    sf::Vector2f mouseWorld = mWindow.mapPixelToCoords(mousePos, mView);

    // Clone from Player's inventory
    std::unique_ptr<Weapon> clonedWeapon = mPlayer.getThrowableWeapon().clone();

    auto grenade = std::unique_ptr<ThrowableWeapon>(dynamic_cast<ThrowableWeapon*>(clonedWeapon.release()));

    if (!grenade) {
        // This indicates a logic error in your inventory system
        throw GameException("Logic Error: Attempted to throw a non-throwable weapon.");
    }

    grenade->throwAt(mouseWorld);
    mActiveGrenades.push_back(std::move(grenade));
    std::cout << "Grenade thrown! Remaining: " << mInventory.getGrenadeCount() << "\n";
}

void Game::update(sf::Time deltaTime) {
    if (mPlayer.isDead()) {
        mIsGameOver = true;
        return;
    }

    mWindow.setView(mView);

    mPlayer.update(deltaTime, mWorldSize, mWindow);
    mHUD.update(mPlayer, mWindow, deltaTime, mCoins, mInventory);

    handleEnemySpawning(deltaTime);

    for (const auto& enemy : mEnemies) {
        enemy->update(deltaTime, mWorldSize, mWindow);
    }

    updateProjectiles(deltaTime);
    updateEnemyProjectiles(deltaTime);
    updateGrenades(deltaTime);

    resolveEnemyCollisions();
    checkCollisions();
    cleanupEntities();
    updateCamera();

    if (mUpgradeClock.getElapsedTime().asSeconds() >= 30.0f) {
        std::cout << "\n=== GAME WORLD LEVEL UP! ===\n";

        // Upgrade all player weapons
        mPlayer.getRangedWeapon().applyUpgrade();
        mPlayer.getMeleeWeapon().applyUpgrade();
        mPlayer.getThrowableWeapon().applyUpgrade();
        mPlayer.getMagicWeapon().applyUpgrade();

        // Reset the timer
        mUpgradeClock.restart();
    }

    //Magic weapons update
    MagicWeapon& magic = mPlayer.getMagicWeapon();
    magic.update(deltaTime);

    if (magic.shouldDealDamage()) {
        float damage = magic.getDamage();
        std::cout << ">>> MAGIC TICK: Dealing " << damage << " to all enemies.\n";

        for (const auto& enemyPtr : mEnemies) {
            // Safe cast check
            if (auto* enemy = dynamic_cast<Enemy*>(enemyPtr.get())) {
                if (enemy->getCurrentHealth() > 0) {
                    enemy->takeDamage(damage);
                }
            }
        }
    }
}

void Game::updateProjectiles(sf::Time deltaTime) {
    for (auto& proj : mProjectiles) {
        proj.update(deltaTime, mWorldSize);
    }
}

void Game::updateEnemyProjectiles(sf::Time deltaTime) {
    for (auto& proj : mEnemyProjectiles) {
        proj.update(deltaTime, mWorldSize);
    }
}

void Game::updateCamera() {
    sf::Vector2f targetPos = mPlayer.getPlayerPosition();
    sf::Vector2u playerSize = mPlayer.getTextureSize();

    targetPos.x += static_cast<float>(playerSize.x) / 2.f;
    targetPos.y += static_cast<float>(playerSize.y) / 2.f;

    sf::Vector2f viewSize = mView.getSize();
    float halfW = viewSize.x / 2.0f;
    float halfH = viewSize.y / 2.0f;

    if (targetPos.x < halfW) targetPos.x = halfW;
    else if (targetPos.x > mWorldSize.x - halfW) targetPos.x = mWorldSize.x - halfW;

    if (targetPos.y < halfH) targetPos.y = halfH;
    else if (targetPos.y > mWorldSize.y - halfH) targetPos.y = mWorldSize.y - halfH;

    mView.setCenter(targetPos);
}

void Game::checkCollisions() {
    // Check collisions for player projectiles with enemies
    for (auto& proj : mProjectiles) {
        if (proj.isDestroyed()) continue;

        for (const auto& enemyPtr : mEnemies) {

            auto* enemyA = dynamic_cast<Enemy*>(enemyPtr.get());
            if (enemyPtr->getCurrentHealth() <= 0) continue;
            if (proj.getBounds().findIntersection(enemyPtr->getGlobalBounds())) {
                enemyA->takeDamage(proj.getDamage());
                proj.destroy();
                break;
            }
        }
    }

    // Check collisions for enemy projectiles with player
    sf::FloatRect playerBounds = mPlayer.getGlobalBounds();
    for (auto& enemyProj : mEnemyProjectiles) {
        if (enemyProj.isDestroyed()) continue;

        if (enemyProj.getBounds().findIntersection(playerBounds)) {
            mPlayer.takeDamage(enemyProj.getDamage());
            enemyProj.destroy();
        }
    }
}

void Game::updateGrenades(sf::Time deltaTime) {
    for (auto& grenade : mActiveGrenades) {
        grenade->update(deltaTime);

        if (grenade->shouldDealDamage()) {
            sf::Vector2f explosionPos = grenade->getPosition();
            float radius = grenade->getExplosionRadius();
            float damage = grenade->getDamage();

            std::cout << "BOOM! Damage: " << damage << " Radius: " << radius << "\n";

            for (auto& enemyPtr : mEnemies) {
                if (enemyPtr->getCurrentHealth() <= 0) continue;

                sf::Vector2f ePos = enemyPtr->getPosition();
                sf::Vector2u eSize = enemyPtr->getSpriteSize();
                sf::Vector2f eCenter = ePos + sf::Vector2f(static_cast<float>(eSize.x)/2.f, static_cast<float>(eSize.y)/2.f);

                float dx = eCenter.x - explosionPos.x;
                float dy = eCenter.y - explosionPos.y;
                float dist = std::sqrt(dx*dx + dy*dy);

                const auto enemyA = dynamic_cast<Enemy*>(enemyPtr.get());
                if (dist <= radius) {
                    enemyA->takeDamage(damage);
                }
            }

            sf::Vector2f pPos = mPlayer.getPlayerPosition();
            sf::Vector2u pSize = mPlayer.getTextureSize();
            sf::Vector2f pCenter = pPos + sf::Vector2f(static_cast<float>(pSize.x)/2.f, static_cast<float>(pSize.y)/2.f);

            float pdx = pCenter.x - explosionPos.x;
            float pdy = pCenter.y - explosionPos.y;
            float pDist = std::sqrt(pdx*pdx + pdy*pdy);

            if (pDist <= radius) {
                mPlayer.takeDamage(damage);
            }
        }
    }
}

void Game::cleanupEntities() {
    std::erase_if(mEnemies, [this](const auto& enemy) {
        if (enemy->getCurrentHealth() <= 0) {
            Enemy::death();
            mCoins += 10;  // Award 10 coins for each kill
            std::cout << "Enemy killed! +10 Coins. Total: " << mCoins << "\n";
            return true;
        }
        return false;
    });

    std::erase_if(mProjectiles, [](const Projectile& p) {
        return p.isDestroyed();
    });

    std::erase_if(mEnemyProjectiles, [](const EnemyProjectile& p) {
        return p.isDestroyed();
    });

    std::erase_if(mActiveGrenades, [](const auto& g) {
        return g->isFinished();
    });
}

void Game::resolveEnemyCollisions() const {
    for (size_t i = 0; i < mEnemies.size(); ++i) {
        for (size_t j = i + 1; j < mEnemies.size(); ++j) {
            auto* enemyA = mEnemies[i].get();
            auto* enemyB = mEnemies[j].get();

            sf::Vector2f posA = enemyA->getPosition();
            sf::Vector2f centerA = posA + sf::Vector2f(static_cast<float>(enemyA->getSpriteSize().x) / 2.f, static_cast<float>(enemyA->getSpriteSize().y) / 2.f);

            sf::Vector2f posB = enemyB->getPosition();
            sf::Vector2f centerB = posB + sf::Vector2f(static_cast<float>(enemyB->getSpriteSize().x) / 2.f, static_cast<float>(enemyB->getSpriteSize().y) / 2.f);

            sf::Vector2f diff = centerA - centerB;
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            float minDistance = (static_cast<float>(enemyA->getSpriteSize().x + enemyB->getSpriteSize().x)) * 0.4f;

            if (distance < minDistance) {
                sf::Vector2f pushVector = (distance == 0.f) ? sf::Vector2f(1.f, 0.f) : (diff / distance);
                float pushStrength = (minDistance - distance) / 2.f;

                enemyA->setPosition(posA + (pushVector * pushStrength));
                enemyB->setPosition(posB - (pushVector * pushStrength));
            }
        }
    }
}

void Game::render() {
    mWindow.clear(sf::Color::Black);

    mWindow.setView(mView);
    mLevel.render(mWindow);

    if (!mPlayer.isDead()) {
        mPlayer.render(mWindow);
    }

    for (const auto& enemy : mEnemies) enemy->render(mWindow);
    for (const auto& proj : mProjectiles) proj.render(mWindow);
    for (const auto& enemyProj : mEnemyProjectiles) enemyProj.render(mWindow);
    for (const auto& g : mActiveGrenades) g->render(mWindow);

    mWindow.setView(mWindow.getDefaultView());
    mHUD.render(mWindow);

    if (mIsGameOver) {
        mWindow.setView(mWindow.getDefaultView());
        mHUD.renderGameOver(mWindow);
    }

    mWindow.display();
}

void Game::handleEnemySpawning(sf::Time deltaTime) {
    mSpawnTimer += deltaTime.asSeconds();
    float totalTime = mSurvivalClock.getElapsedTime().asSeconds();

    if (totalTime < 30.f) mSpawnInterval = 2.5f;
    else if (totalTime < 60.f) mSpawnInterval = 1.5f;
    else if (totalTime < 90.f) mSpawnInterval = 0.8f;
    else mSpawnInterval = 0.6f;

    if (mSpawnTimer >= mSpawnInterval) {
        spawnOneEnemy();
        mSpawnTimer = 0.f;
    }
}

void Game::spawnOneEnemy() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(100.f, mWorldSize.x - 100.f);
    std::uniform_real_distribution<float> distY(100.f, mWorldSize.y - 100.f);
    std::uniform_int_distribution<int> typeSelector(0, 1); // 0 = Melee, 1 = Ranged

    sf::Vector2f spawnPos;
    sf::Vector2f playerPos = mPlayer.getPlayerPosition();
    bool validPosition = false;
    int attempts = 0;

    while (!validPosition && attempts < 10) {
        spawnPos.x = distX(gen);
        spawnPos.y = distY(gen);
        float dx = spawnPos.x - playerPos.x;
        float dy = spawnPos.y - playerPos.y;
        float distance = std::sqrt(dx*dx + dy*dy);

        if (distance > 600.f) validPosition = true;
        attempts++;
    }

    // Randomly select enemy type
    EnemyType enemyType = (typeSelector(gen) == 0) ? EnemyType::Melee : EnemyType::Ranged;

    // Spawn with different stats based on type
    std::unique_ptr<Enemy> newEnemy;
    if (enemyType == EnemyType::Melee) {
        // Melee: Higher HP, faster, closer range
        newEnemy = std::make_unique<MeleeEnemy>(spawnPos);
    } else {
        // Ranged: Lower HP, slower, longer range (handled by weapon stats)
        newEnemy = std::make_unique<RangedEnemy>(spawnPos);
    }

    // Set game reference before adding to vector
    newEnemy->setGameReference(this);
    mEnemies.push_back(std::move(newEnemy));
}

// Shop system implementation
void Game::handleShopInput() {
    // Key bindings for shop:
    // 1 - Buy Grenade
    // 2 - Buy Health Potion
    // 3 - Buy Damage Boost
    // 4 - Buy Magic Damage
    // Left Mouse - Use Grenade
    // P - Use Health Potion
    // L - Use Magic Damage

    static sf::Clock shopCooldown;
    const float SHOP_COOLDOWN = 0.3f;  // Prevent spam

    if (shopCooldown.getElapsedTime().asSeconds() < SHOP_COOLDOWN) return;

    // BUY inputs (number keys)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
        buyGrenade();
        shopCooldown.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
        buyHealthPotion();
        shopCooldown.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
        buyDamageBoost();
        shopCooldown.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
        buyMagicDamage();
        shopCooldown.restart();
    }

    // USE inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
        useHealthPotion();
        shopCooldown.restart();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
        useMagicDamage();
        shopCooldown.restart();
    }
    // Grenade usage is handled by left mouse click in handleGrenadeThrow()
}

void Game::buyGrenade() {
    if (mCoins >= GRENADE_COST) {
        mCoins -= GRENADE_COST;
        mInventory.addGrenade(1);
        std::cout << "Grenade purchased! Cost: " << GRENADE_COST << " Coins. Remaining: " << mCoins << "\n";
    } else {
        std::cout << "Not enough coins! Need " << GRENADE_COST << " coins, have " << mCoins << "\n";
    }
}

void Game::buyHealthPotion() {
    if (mCoins >= HEALTH_POTION_COST) {
        mCoins -= HEALTH_POTION_COST;
        mInventory.addHealthPotion(1);
        std::cout << "Health Potion purchased! Cost: " << HEALTH_POTION_COST << " Coins. Remaining: " << mCoins << "\n";
    } else {
        std::cout << "Not enough coins! Need " << HEALTH_POTION_COST << " coins, have " << mCoins << "\n";
    }
}

void Game::buyDamageBoost() {
    if (mCoins >= DAMAGE_BOOST_COST) {
        mCoins -= DAMAGE_BOOST_COST;
        std::cout << "Damage Boost purchased! Cost: " << DAMAGE_BOOST_COST << " Coins. Remaining: " << mCoins << "\n";

        // Apply 10% damage boost to all weapons
        float meleeOldDamage = mPlayer.getMeleeWeapon().getDamage();
        float rangedOldDamage = mPlayer.getRangedWeapon().getDamage();

        mPlayer.getMeleeWeapon().setDamage(meleeOldDamage * DAMAGE_BOOST_MULTIPLIER);
        mPlayer.getRangedWeapon().setDamage(rangedOldDamage * DAMAGE_BOOST_MULTIPLIER);

        std::cout << "All weapons boosted by 10%!\n";
        std::cout << "Melee: " << meleeOldDamage << " -> " << mPlayer.getMeleeWeapon().getDamage() << "\n";
        std::cout << "Ranged: " << rangedOldDamage << " -> " << mPlayer.getRangedWeapon().getDamage() << "\n";
    } else {
        std::cout << "Not enough coins! Need " << DAMAGE_BOOST_COST << " coins, have " << mCoins << "\n";
    }
}

void Game::buyMagicDamage() {
    if (mCoins >= MAGIC_DAMAGE_COST) {
        mCoins -= MAGIC_DAMAGE_COST;
        mInventory.addDamagePotion(1);
        std::cout << "Magic Damage purchased! Cost: " << MAGIC_DAMAGE_COST << " Coins. Remaining: " << mCoins << "\n";
    } else {
        std::cout << "Not enough coins! Need " << MAGIC_DAMAGE_COST << " coins, have " << mCoins << "\n";
    }
}

void Game::useHealthPotion() {
    if (mInventory.useHealthPotion()) {
        mPlayer.heal(20.f);  // Heals 20 HP
        std::cout << "Health Potion used! +20 HP\n";
    } else {
        std::cout << "No health potions in inventory!\n";
    }
}

void Game::useMagicDamage() {
    if (mInventory.useDamagePotion()) {
        // Damage all enemies using MagicWeapon
        float magicDamage = 50.f;  // Base magic damage
        std::cout << "Magic Damage activated! Damaging all enemies...\n";

        int enemiesHit = 0;
        for (auto& enemy : mEnemies) {
            enemy->takeDamage(magicDamage);
            enemiesHit++;
        }

        std::cout << "Magic Damage hit " << enemiesHit << " enemies for " << magicDamage << " damage each!\n";
    } else {
        std::cout << "No magic damage in inventory!\n";
    }
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "====== GAME STATE ======\n"
       << "* " << game.mLevel << "\n"
       << "* " << game.mHUD << "\n"
       << "* " << game.mPlayer << "\n"
       << "* Enemies: " << game.mEnemies.size() << "\n"
       << "========================\n";
    return os;
}