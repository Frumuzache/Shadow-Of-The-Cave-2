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
      mClock{},
      mSpawnTimer(0.f),
      mSpawnInterval(3.0f),
      mIsGameOver(false)
{
    mWindow.setFramerateLimit(120);

    // Initialize first Enemies
    mEnemies.push_back(std::make_unique<Enemy>(sf::Vector2f(300.f, 100.f), 175.f, 75.f));
    mEnemies.push_back(std::make_unique<Enemy>(sf::Vector2f(600.f, 400.f), 175.f, 75.f));
    mEnemies.push_back(std::make_unique<Enemy>(sf::Vector2f(900.f, 700.f), 175.f, 75.f));

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

void Game::handleMeleeAttack() {
    static sf::Clock meleeClock;

    const Weapon& currentWeapon = mPlayer.getMeleeWeapon();

    if (meleeClock.getElapsedTime().asSeconds() < currentWeapon.getReloadTime()) return;
    meleeClock.restart();

    sf::Vector2f playerPos = mPlayer.getPlayerPosition();
    sf::Vector2u pSize = mPlayer.getTextureSize();
    sf::Vector2f playerCenter = { playerPos.x + static_cast<float>(pSize.x) / 2.f, playerPos.y + static_cast<float>(pSize.y) / 2.f };

    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
    sf::Vector2f mouseWorld = mWindow.mapPixelToCoords(mousePos);
    sf::Vector2f aimDir = mouseWorld - playerCenter;
    float len = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);
    if (len != 0) aimDir /= len;

    for (const auto& enemyPtr : mEnemies) {
        if (enemyPtr->getCurrentHealth() <= 0) continue;

        const sf::Vector2f enemyPos = enemyPtr->getPosition();
        const sf::Vector2u eSize = enemyPtr->getSpriteSize();
        const sf::Vector2f enemyCenter = enemyPos + sf::Vector2f(static_cast<float>(eSize.x)/2.f, static_cast<float>(eSize.y)/2.f);

        sf::Vector2f diff = enemyCenter - playerCenter;
        float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

        if (dist <= currentWeapon.getRange()) {
            sf::Vector2f dirToEnemy = diff / dist;
            float dotProduct = aimDir.x * dirToEnemy.x + aimDir.y * dirToEnemy.y;
            if (dotProduct > 0.5f) {
                enemyPtr->takeDamage(currentWeapon.getDamage());
                std::cout << "Melee hit!\n";
            }
        }
    }
}

void Game::handleGrenadeThrow() {
    if (mGrenadeCooldown.getElapsedTime().asSeconds() < 2.0f) return;
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
    mGrenadeCooldown.restart();
}

void Game::update(sf::Time deltaTime) {
    if (mPlayer.isDead()) {
        mIsGameOver = true;
        return;
    }

    mWindow.setView(mView);

    mPlayer.update(deltaTime, mWorldSize, mWindow);
    mHUD.update(mPlayer, mWindow, deltaTime);

    handleEnemySpawning(deltaTime);

    for (const auto& enemy : mEnemies) {
        enemy->update(deltaTime, mWorldSize, mWindow);
    }

    updateProjectiles(deltaTime);
    updateGrenades(deltaTime);

    resolveEnemyCollisions();
    checkCollisions();
    cleanupEntities();
    updateCamera();
}

void Game::updateProjectiles(sf::Time deltaTime) {
    for (auto& proj : mProjectiles) {
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
    for (auto& proj : mProjectiles) {
        if (proj.isDestroyed()) continue;

        for (const auto& enemy : mEnemies) {
            if (enemy->getCurrentHealth() <= 0) continue;
            if (proj.getBounds().findIntersection(enemy->getGlobalBounds())) {
                enemy->takeDamage(proj.getDamage());
                proj.destroy();
                break;
            }
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

            for (auto& enemy : mEnemies) {
                if (enemy->getCurrentHealth() <= 0) continue;

                sf::Vector2f ePos = enemy->getPosition();
                sf::Vector2u eSize = enemy->getSpriteSize();
                sf::Vector2f eCenter = ePos + sf::Vector2f(static_cast<float>(eSize.x)/2.f, static_cast<float>(eSize.y)/2.f);

                float dx = eCenter.x - explosionPos.x;
                float dy = eCenter.y - explosionPos.y;
                float dist = std::sqrt(dx*dx + dy*dy);

                if (dist <= radius) {
                    enemy->takeDamage(damage);
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
    std::erase_if(mEnemies, [](const auto& enemy) {
        if (enemy->getCurrentHealth() <= 0) {
            enemy->death();
            return true;
        }
        return false;
    });

    std::erase_if(mProjectiles, [](const Projectile& p) {
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

    mEnemies.push_back(std::make_unique<Enemy>(spawnPos, 175.f, 75.f));
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