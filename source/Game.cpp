#include <SFML/Graphics.hpp>
#include "../header/Game.h"
#include "../header/Player.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <algorithm> // For std::remove_if

Game::Game(unsigned int width, unsigned int height, const std::string& title)
    : mWindow(sf::VideoMode({width, height}), title),
      mClock{},
      mLevel("../assets/background.png", sf::Vector2f(3000.f, 3000.f)),
      mHUD("../assets/arial.ttf"),
      mPlayer(Player::getInstance())
{
    mWindow.setFramerateLimit(120);

    // Initialize Enemies
    mEnemies.push_back(std::make_unique<Enemy>(sf::Vector2f(300.f, 100.f), 80.f, 50.f));
    mEnemies.push_back(std::make_unique<Enemy>(sf::Vector2f(600.f, 400.f), 80.f, 50.f));
    mEnemies.push_back(std::make_unique<Enemy>(sf::Vector2f(900.f, 700.f), 80.f, 50.f));

    // Initialize World & View
    mWorldSize = sf::Vector2f(3000.f, 3000.f);
    mView.setSize({static_cast<float>(width), static_cast<float>(height)});
    mView.setCenter({static_cast<float>(width) / 2.f, static_cast<float>(height) / 2.f});

    // Initialize Weapon
    Weapon rifle("AK-47", 15.f, 0.1f, 800.f, WeaponType::Ranged);
    rifle.loadTexture("../assets/rifle.png");
    rifle.setVisualSize(200.f, 100.f); // Reasonable size for the gun
    mPlayer.getWeapon() = rifle;

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

// 1. Process Window Events (Close, Resize, etc.)
void Game::processEvents() {
    // Ensure we use the game view for any input processing here if needed
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

    // Handle Continuous Input (Movement, Shooting) separate from the event loop
    handleInput();
}

// 2. Handle Real-time Input (Keyboard/Mouse)
void Game::handleInput() {
    // Melee Attack (Space)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        handleMeleeAttack();
    }

    // Shooting (Left Click)
    if (isButtonPressed(sf::Mouse::Button::Left)) {
        handleShooting();
    }
}

// 3. Logic for Shooting Logic
void Game::handleShooting() {
    static sf::Clock fireClock;
    Weapon& currentWeapon = mPlayer.getWeapon();

    // Check Cooldown
    if (fireClock.getElapsedTime().asSeconds() < currentWeapon.getReloadTime()) {
        return;
    }

    // Only fire if it's a Ranged weapon
    if (currentWeapon.getType() != WeaponType::Ranged) {
        return;
    }

    fireClock.restart();

    // Calculate Firing positions
    sf::Vector2f playerPos = mPlayer.getPlayerPosition();
    sf::Vector2u playerSize = mPlayer.getTextureSize();
    sf::Vector2f playerCenter = {
        playerPos.x + static_cast<float>(playerSize.x) / 2.f,
        playerPos.y + static_cast<float>(playerSize.y) / 2.f
    };

    // Calculate Mouse Direction relative to world
    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
    sf::Vector2f mouseWorld = mWindow.mapPixelToCoords(mousePos);
    sf::Vector2f directionVec = mouseWorld - playerCenter;

    float len = std::sqrt(directionVec.x * directionVec.x + directionVec.y * directionVec.y);
    sf::Vector2f normalizedDir = (len != 0) ? (directionVec / len) : sf::Vector2f(1, 0);

    // Spawn Projectile(s)
    if (currentWeapon.getName() == "Shotgun") {
        // Shotgun Spread Logic
        for (int i = -1; i <= 1; ++i) {
            float angleOffset = static_cast<float>(i) * 0.15f;
            float sn = std::sin(angleOffset);
            float cs = std::cos(angleOffset);
            sf::Vector2f spreadDir = {
                normalizedDir.x * cs - normalizedDir.y * sn,
                normalizedDir.x * sn + normalizedDir.y * cs
            };
            mProjectiles.emplace_back(playerCenter, spreadDir, 1000.f, currentWeapon.getDamage());
        }
    } else {
        // Standard Fire
        mProjectiles.emplace_back(playerCenter, normalizedDir, 1000.f, currentWeapon.getDamage());
    }
}

// 4. Logic for Melee Attacks
void Game::handleMeleeAttack() {
    static sf::Clock meleeClock;
    Weapon& currentWeapon = mPlayer.getWeapon();

    if (currentWeapon.getType() != WeaponType::Melee) return;
    if (meleeClock.getElapsedTime().asSeconds() < currentWeapon.getReloadTime()) return;

    meleeClock.restart();

    // Get Mouse Direction for Aiming
    sf::Vector2f playerPos = mPlayer.getPlayerPosition();
    sf::Vector2u pSize = mPlayer.getTextureSize();
    sf::Vector2f playerCenter = { playerPos.x + static_cast<float>(pSize.x) / 2.f, playerPos.y + static_cast<float>(pSize.y) / 2.f };

    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
    sf::Vector2f mouseWorld = mWindow.mapPixelToCoords(mousePos);
    sf::Vector2f aimDir = mouseWorld - playerCenter;
    float len = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);
    if (len != 0) aimDir /= len;

    // Check all enemies
    for (const auto& enemyPtr : mEnemies) {
        if (enemyPtr->getCurrentHealth() <= 0) continue;

        sf::Vector2f enemyPos = enemyPtr->getPosition();
        sf::Vector2u eSize = enemyPtr->getSpriteSize();
        sf::Vector2f enemyCenter = enemyPos + sf::Vector2f(static_cast<float>(eSize.x)/2.f, static_cast<float>(eSize.y)/2.f);

        sf::Vector2f diff = enemyCenter - playerCenter;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        // Check Range
        if (dist <= currentWeapon.getRange()) {
            // Check Angle (Are we facing the enemy?)
            sf::Vector2f dirToEnemy = diff / dist;
            float dotProduct = aimDir.x * dirToEnemy.x + aimDir.y * dirToEnemy.y;

            if (dotProduct > 0.5f) { // ~60 degree cone
                enemyPtr->takeDamage(currentWeapon.getDamage());
                std::cout << "Melee hit!\n";
            }
        }
    }
}

// 5. Main Update Loop
void Game::update(sf::Time deltaTime) {
    // Set View FIRST so all logic uses correct coordinates
    mWindow.setView(mView);

    // Update Entities
    mPlayer.update(deltaTime, mWorldSize, mWindow);
    mHUD.update(mPlayer);

    for (auto& enemy : mEnemies) {
        enemy->update(deltaTime, mWorldSize, mWindow);
    }

    // Update Projectiles
    updateProjectiles(deltaTime);

    // Physics & Logic
    resolveEnemyCollisions();
    checkCollisions();     // Bullet vs Enemy
    cleanupEntities();     // Remove dead stuff
    updateCamera();        // Move view
}

void Game::updateProjectiles(sf::Time deltaTime) {
    for (auto& proj : mProjectiles) {
        proj.update(deltaTime, mWorldSize);
    }
}

// 6. Camera Logic
void Game::updateCamera() {
    sf::Vector2f targetPos = mPlayer.getPlayerPosition();
    sf::Vector2u playerSize = mPlayer.getTextureSize();

    // Center on Player
    targetPos.x += static_cast<float>(playerSize.x) / 2.f;
    targetPos.y += static_cast<float>(playerSize.y) / 2.f;

    sf::Vector2f viewSize = mView.getSize();
    float halfW = viewSize.x / 2.0f;
    float halfH = viewSize.y / 2.0f;

    // Clamp Camera to World Bounds
    if (targetPos.x < halfW) targetPos.x = halfW;
    else if (targetPos.x > mWorldSize.x - halfW) targetPos.x = mWorldSize.x - halfW;

    if (targetPos.y < halfH) targetPos.y = halfH;
    else if (targetPos.y > mWorldSize.y - halfH) targetPos.y = mWorldSize.y - halfH;

    mView.setCenter(targetPos);
}

// 7. Collision Detection (Bullet vs Enemy)
void Game::checkCollisions() {
    for (auto& proj : mProjectiles) {
        if (proj.isDestroyed()) continue;

        for (auto& enemy : mEnemies) {
            if (enemy->getCurrentHealth() <= 0) continue;

            // SFML 3 Intersection Check
            if (proj.getBounds().findIntersection(enemy->getGlobalBounds())) {
                enemy->takeDamage(proj.getDamage());
                proj.destroy();
                break; // One bullet hits one enemy
            }
        }
    }
}

// 8. Cleanup Dead Objects
void Game::cleanupEntities() {
    // Remove dead enemies
    std::erase_if(mEnemies, [](const auto& enemy) {
        if (enemy->getCurrentHealth() <= 0) {
            enemy->death();
            return true;
        }
        return false;
    });

    // Remove destroyed projectiles
    std::erase_if(mProjectiles, [](const Projectile& p) {
        return p.isDestroyed();
    });
}

// 9. Enemy Separation Logic
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

// 10. Render
void Game::render() {
    mWindow.clear(sf::Color::Black);

    // Draw World
    mWindow.setView(mView);
    mLevel.render(mWindow);
    mPlayer.render(mWindow);
    for (const auto& enemy : mEnemies) enemy->render(mWindow);
    for (const auto& proj : mProjectiles) proj.render(mWindow);

    // Draw HUD
    mWindow.setView(mWindow.getDefaultView());
    mHUD.render(mWindow);

    mWindow.display();
}

// Operator overload
std::ostream& operator<<(std::ostream& os, const Game& game) {
    os << "====== GAME STATE ======\n"
       << "* " << game.mLevel << "\n"
       << "* " << game.mHUD << "\n"
       << "* " << game.mPlayer << "\n"
       << "* Enemies: " << game.mEnemies.size() << "\n"
       << "========================\n";
    return os;
}