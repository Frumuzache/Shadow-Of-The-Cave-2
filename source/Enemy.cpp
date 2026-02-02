#include "../header/Enemy.h"
#include "../header/Player.h" // Required to access Player singleton
#include "../header/Game.h"   // For fireProjectile() to access mEnemyProjectiles
#include "../header/EnemyProjectile.h"
#include <iostream>
#include <cmath>
#include "../header/GameException.h"

// Default Constructor
Enemy::Enemy()
    : Entity(200.f, 50.f),
      mDirection(0.f, 0.f),
      initialPosition(100.f, 100.f),
      mEnemyType(EnemyType::Melee),
      mGame(nullptr),
      mWeapon(nullptr)
{
    // Initialize with melee weapon by default
    mWeapon = std::make_unique<MeleeWeapon>("Zombie Claws", 15.f, 1.0f, 80.f);

    loadAssets();
    mSprite.setPosition(initialPosition);

    mHealthBarBackground.setSize(sf::Vector2f(100.f, 50.f));
    mHealthBarBackground.setFillColor(sf::Color(50, 50, 50));
    mHealthBarBackground.setOutlineThickness(1.f);
    mHealthBarBackground.setOutlineColor(sf::Color::Black);

    mHealthBarForeground.setSize(sf::Vector2f(50.f, 5.f));
    mHealthBarForeground.setFillColor(sf::Color::Green);
}

// Parameterized Constructor
Enemy::Enemy(const sf::Vector2f startPosition, const float speed, const float health, EnemyType type)
    : Entity(speed, health),
      mDirection(0.f, 0.f),
      initialPosition(startPosition),
      mEnemyType(type),
      mGame(nullptr),
      mWeapon(nullptr)
{
    // Initialize weapon based on enemy type
    if (mEnemyType == EnemyType::Melee) {
        mWeapon = std::make_unique<MeleeWeapon>("Zombie Claws", 15.f, 1.0f, 80.f);
    } else {
        // For Ranged: Create a placeholder - RangedEnemy will override with initializeRifle()
        mWeapon = std::make_unique<RangedWeapon>("Skeleton Bow", 20.f, 1.5f, 500.f);
        std::cout << "DEBUG: Enemy created as Ranged type - placeholder weapon set\n";
    }

    loadAssets();
    mSprite.setPosition(startPosition);

    mHealthBarBackground.setSize(sf::Vector2f(200.f, 25.f));
    mHealthBarBackground.setFillColor(sf::Color(50, 50, 50));
    mHealthBarBackground.setOutlineThickness(1.f);
    mHealthBarBackground.setOutlineColor(sf::Color::Black);

    mHealthBarForeground.setSize(sf::Vector2f(50.f, 5.f));
    mHealthBarForeground.setFillColor(sf::Color::Green);
}

void Enemy::loadAssets() {

    // For now, both use the same texture. You can:
    // 1. Create a different colored version of enemy.png as enemy_ranged.png
    // 2. OR duplicate enemy.png and rename it to enemy_ranged.png
    // 3. OR create a completely different design for ranged enemies

    std::string texturePath = "../assets/enemy.png";  // Both types use this for now

    if (!mTexture.loadFromFile(texturePath)) {
        throw AssetLoadException("Enemy Texture", texturePath);
    }
    mSprite.setTexture(mTexture, true);

    if (mEnemyType == EnemyType::Ranged) {
        mSprite.setColor(sf::Color(255, 200, 100)); // Orange tint for ranged enemies
    }
}

// Main Update Loop
void Enemy::update(const sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) {
    // 1. Handle Movement
    updateMovementEnemy(deltaTime, mapBounds);

    // 2. Update weapon rotation for ranged enemies
    if (mEnemyType == EnemyType::Ranged) {
        updateWeaponRotation();
    }

    // 3. Handle Combat (Try to attack player)
    tryAttack();
    updateHealthBarVisuals();

    // Suppress unused variable warning if window isn't used for logic
    (void)window;
}

// Combat Logic
void Enemy::tryAttack() {
    // Access the Player Singleton
    Player& player = Player::getInstance();

    // Optimization: Don't attack if player is already dead
    if (player.isDead()) return;

    // 1. Calculate Centers for accurate distance - CHECKED EVERY FRAME
    sf::Vector2f playerPos = player.getPlayerPosition();
    sf::Vector2u pSize = player.getTextureSize();
    sf::Vector2f playerCenter = playerPos + sf::Vector2f(static_cast<float>(pSize.x) * 0.5f, static_cast<float>(pSize.y) * 0.5f);

    sf::Vector2f enemyPos = mSprite.getPosition();
    sf::Vector2u eSize = mTexture.getSize();
    sf::Vector2f enemyCenter = enemyPos + sf::Vector2f(static_cast<float>(eSize.x) * 0.5f, static_cast<float>(eSize.y) * 0.5f);

    // 2. Calculate Distance - RECALCULATED EVERY FRAME
    sf::Vector2f diff = playerCenter - enemyCenter;
    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    // 3. Check Range (using weapon stats) - VERIFIED CONSTANTLY
    if (distance <= mWeapon->getRange()) {
        // 4. Check Cooldown - ATTACKS AS SOON AS COOLDOWN IS READY
        if (mAttackClock.getElapsedTime().asSeconds() >= mWeapon->getReloadTime()) {
            // Different attack based on type
            if (mEnemyType == EnemyType::Melee) {
                // Direct damage
                player.takeDamage(mWeapon->getDamage());
                mAttackClock.restart();

                std::cout << "Melee Enemy attacks! Distance: " << distance << "px\n";
            } else {
                // Fire projectile for ranged enemies
                fireProjectile();
                mAttackClock.restart();

                std::cout << "Ranged Enemy fires! Distance: " << distance << "px | Weapon Range: " << mWeapon->getRange() << "px\n";
            }
        }
    } else {
        // Debug: Show why ranged enemy is NOT firing
        if (mEnemyType == EnemyType::Ranged) {
            static int debugCounter3 = 0;
            if (debugCounter3++ % 120 == 0) {  // Print every 120 frames
                std::cout << "Ranged Enemy OUT OF RANGE! Distance: " << distance << "px | Weapon Range: " << mWeapon->getRange() << "px\n";
            }
        }
    }
}

// Fire projectile towards player
void Enemy::fireProjectile() {
    if (!mGame) return; // Safety check

    Player& player = Player::getInstance();
    sf::Vector2f playerPos = player.getPlayerPosition();
    sf::Vector2u pSize = player.getTextureSize();
    sf::Vector2f playerCenter = playerPos + sf::Vector2f(static_cast<float>(pSize.x) * 0.5f, static_cast<float>(pSize.y) * 0.5f);

    sf::Vector2f enemyPos = mSprite.getPosition();
    sf::Vector2u eSize = mTexture.getSize();
    sf::Vector2f enemyCenter = enemyPos + sf::Vector2f(static_cast<float>(eSize.x) * 0.5f, static_cast<float>(eSize.y) * 0.5f);

    // Calculate direction towards player
    sf::Vector2f direction = playerCenter - enemyCenter;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0.f) {
        direction /= distance; // Normalize
    }

    // Create projectile with SLOWER SPEED than player (700 units/s)
    EnemyProjectile newProjectile(enemyCenter, direction, 700.f, mWeapon->getDamage());
    mGame->addEnemyProjectile(newProjectile);
}

// Getter for enemy type
EnemyType Enemy::getEnemyType() const {
    return mEnemyType;
}

// Update weapon rotation towards player for ranged enemies
void Enemy::updateWeaponRotation() {
    if (!mWeapon) return;

    Player& player = Player::getInstance();
    sf::Vector2f playerPos = player.getPlayerPosition();
    sf::Vector2u pSize = player.getTextureSize();
    sf::Vector2f playerCenter = playerPos + sf::Vector2f(static_cast<float>(pSize.x) * 0.5f, static_cast<float>(pSize.y) * 0.5f);

    sf::Vector2f enemyPos = mSprite.getPosition();
    sf::Vector2u eSize = mTexture.getSize();
    sf::Vector2f enemyCenter = enemyPos + sf::Vector2f(static_cast<float>(eSize.x) * 0.5f, static_cast<float>(eSize.y) * 0.5f);

    // Calculate direction towards player
    sf::Vector2f diff = playerCenter - enemyCenter;
    sf::Angle angle = sf::radians(std::atan2(diff.y, diff.x));

    // Update weapon to face player
    auto* rangedWeapon = dynamic_cast<RangedWeapon*>(mWeapon.get());
    if (rangedWeapon) {
        rangedWeapon->update(enemyCenter, angle);
    }
}

// Movement Logic
void Enemy::updateMovementEnemy(sf::Time deltaTime, const sf::Vector2f& mapBounds) {
    const Player& player = Player::getInstance();

    sf::Vector2f playerPos = player.getPlayerPosition();
    sf::Vector2u pSize = player.getTextureSize();
    sf::Vector2f playerCenter = playerPos + sf::Vector2f(static_cast<float>(pSize.x) * 0.5f, static_cast<float>(pSize.y) * 0.5f);

    sf::Vector2f enemyPos = mSprite.getPosition();
    sf::Vector2u eSize = mTexture.getSize();
    sf::Vector2f enemyCenter = enemyPos + sf::Vector2f(static_cast<float>(eSize.x) * 0.5f, static_cast<float>(eSize.y) * 0.5f);

    sf::Vector2f direction = playerCenter - enemyCenter;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    sf::Vector2f movement(0.f, 0.f);

    // Different movement logic based on enemy type
    if (mEnemyType == EnemyType::Melee) {
        // Melee enemies: Chase player aggressively
        if (distance > 5.0f) {
            movement = (direction / distance) * mMovementSpeed * deltaTime.asSeconds();
        }
    } else {
        // Ranged enemies: Always approach until within 700px range, then STAY and shoot
        // They come TO YOU, not wait for you to come to them!
        constexpr float firingRange = 700.f;  // Approach until this distance (matches weapon range)

        if (distance > firingRange) {
            // Too far! Move closer to get in range (ALWAYS APPROACH)
            movement = (direction / distance) * mMovementSpeed * deltaTime.asSeconds();

            // Debug: Show when ranged enemy is approaching
            static int debugCounter = 0;
            if (debugCounter++ % 60 == 0) {  // Print every 60 frames
                std::cout << "Ranged Enemy approaching... Distance: " << distance << "px\n";
            }
        } else {
            // In range - ready to shoot
            static int debugCounter2 = 0;
            if (debugCounter2++ % 120 == 0) {  // Print every 120 frames
                std::cout << "Ranged Enemy IN RANGE! Distance: " << distance << "px - Ready to fire!\n";
            }
        }
        // Otherwise STAY PUT and shoot (distance <= 700px)
    }

    sf::Vector2f newPos = enemyPos + movement;

    // Map Boundaries Check
    if (newPos.x < 0.f) newPos.x = 0.f;
    if (newPos.y < 0.f) newPos.y = 0.f;
    if (newPos.x + static_cast<float>(eSize.x) > mapBounds.x) newPos.x = mapBounds.x - static_cast<float>(eSize.x);
    if (newPos.y + static_cast<float>(eSize.y) > mapBounds.y) newPos.y = mapBounds.y - static_cast<float>(eSize.y);

    mSprite.setPosition(newPos);
}

void Enemy::takeDamage(float damageAmount) {
    // Modify protected variable 'currentHealth' from Entity
    currentHealth -= damageAmount;
}

void Enemy::death() {
    std::cout << "Enemy has died.\n";
}

void Enemy::render(sf::RenderWindow &window) const {
    window.draw(mSprite);

    // Draw weapon for ranged enemies
    if (mEnemyType == EnemyType::Ranged && mWeapon) {
        mWeapon->render(window);
    }

    if (currentHealth > 0) {
        window.draw(mHealthBarBackground);
        window.draw(mHealthBarForeground);
    }
}

std::ostream& operator<<(std::ostream& os, const Enemy& enemy) {
    os << "--- ENEMY ---\n" << static_cast<const Entity&>(enemy);
    return os;
}


void Enemy::updateHealthBarVisuals() {

    const sf::FloatRect bounds = mSprite.getGlobalBounds();

    constexpr float barMaxWidth = 200.f;
    constexpr float barHeight = 25.f;

    // Calculate center X
    float barX = bounds.position.x + (bounds.size.x / 2.f) - (barMaxWidth / 2.f);

    // Calculate Y (above the sprite)
    float barY = bounds.position.y - 15.f;

    mHealthBarBackground.setPosition({barX, barY});
    mHealthBarForeground.setPosition({barX, barY});

    // --- FIX 2: Prevent "Always Gray" logic errors ---
    // Ensure we don't divide by zero or get negative values
    if (maxHealth <= 0.f) maxHealth = 1.f;

    float hpPercent = currentHealth / maxHealth;
    if (hpPercent < 0.f) hpPercent = 0.f;
    if (hpPercent > 1.f) hpPercent = 1.f;

    mHealthBarForeground.setSize(sf::Vector2f(barMaxWidth * hpPercent, barHeight));

    // Optional: Update color based on health
    if (hpPercent < 0.3f) {
        mHealthBarForeground.setFillColor(sf::Color::Red);
    } else {
        mHealthBarForeground.setFillColor(sf::Color::Green);
    }
}
