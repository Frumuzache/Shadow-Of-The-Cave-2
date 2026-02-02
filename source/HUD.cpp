#include "../header/HUD.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include "../header/GameException.h"

HUD::HUD(std::string fontPath)
: mFontPath(std::move(fontPath)),
  mFont(),
  mCoinsText(static_cast<sf::Text*>(nullptr)),
  mInventoryText(static_cast<sf::Text*>(nullptr)),
  mShopText(static_cast<sf::Text*>(nullptr)),
  mPlayerHealthText(static_cast<sf::Text*>(nullptr)),
  mTimerText(static_cast<sf::Text*>(nullptr)),
  mGameOverText(static_cast<sf::Text*>(nullptr)),
  mLevelClock(),
  mDamageOverlay(),
  mFlashAlpha(0.f),
  mPreviousHealth(100.f)
{
    // Ensure all pointers are initialized (for MSVC checker)
    if (!mFont.openFromFile(mFontPath))
        throw AssetLoadException("HUD Font", mFontPath);

    // Create Text objects with loaded font using new
    mPlayerHealthText = new sf::Text(mFont, "", 24);
    mCoinsText = new sf::Text(mFont, "", 22);
    mInventoryText = new sf::Text(mFont, "", 18);
    mShopText = new sf::Text(mFont, "", 16);
    mTimerText = new sf::Text(mFont, "", 24);
    mGameOverText = new sf::Text(mFont, "GAME OVER", 72);

    // Setup Health
    mPlayerHealthText->setCharacterSize(24);
    mPlayerHealthText->setFillColor(sf::Color::White);
    mPlayerHealthText->setPosition({10.f, 10.f});

    // Setup Coins Text (top-right)
    mCoinsText->setCharacterSize(22);
    mCoinsText->setFillColor(sf::Color::Yellow);

    // Setup Inventory Text (top-right, under coins)
    mInventoryText->setCharacterSize(18);
    mInventoryText->setFillColor(sf::Color::Cyan);

    // Setup Shop Text (right column)
    mShopText->setCharacterSize(16);
    mShopText->setFillColor(sf::Color(220, 220, 220));

    // Setup Timer
    mTimerText->setCharacterSize(24);
    mTimerText->setFillColor(sf::Color::White);

    // Setup Game Over
    mGameOverText->setCharacterSize(72);
    mGameOverText->setFillColor(sf::Color::Red);
    mGameOverText->setStyle(sf::Text::Bold);

    // Setup Damage Flash
    mDamageOverlay.setSize({4000.f, 4000.f});
    mDamageOverlay.setFillColor(sf::Color(255, 0, 0, 0));
}

HUD::~HUD() {
    // Clean up dynamically allocated Text objects
    delete mPlayerHealthText;
    delete mTimerText;
    delete mCoinsText;
    delete mInventoryText;
    delete mShopText;
    delete mGameOverText;
}

void HUD::update(const Player& player, const sf::RenderWindow& window, sf::Time deltaTime, int coins, const Inventory& inventory) {
    // 1. Update Health
    float currentHealth = player.getCurrentHealth();
    mPlayerHealthText->setString("Health: " + std::to_string(static_cast<int>(currentHealth)));

    // 2. Damage Flash Logic
    if (currentHealth < mPreviousHealth) {
        mFlashAlpha = 150.f; // Start flash
    }
    mPreviousHealth = currentHealth;

    if (mFlashAlpha > 0.f) {
        mFlashAlpha -= 300.f * deltaTime.asSeconds();
        if (mFlashAlpha < 0.f) mFlashAlpha = 0.f;

        mDamageOverlay.setFillColor(sf::Color(255, 0, 0, static_cast<std::uint8_t>(mFlashAlpha)));

        sf::Vector2u winSize = window.getSize();
        mDamageOverlay.setSize({static_cast<float>(winSize.x), static_cast<float>(winSize.y)});
        mDamageOverlay.setPosition({0.f, 0.f});
    }

    // 3. Update Timer
    sf::Time elapsed = mLevelClock.getElapsedTime();
    int totalSeconds = static_cast<int>(elapsed.asSeconds());
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;

    mTimerText->setString(ss.str());

    // 4. Center Timer (SFML 3 Fix)
    sf::Vector2u windowSize = window.getSize();
    sf::FloatRect textBounds = mTimerText->getLocalBounds();

    mTimerText->setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    });

    mTimerText->setPosition({static_cast<float>(windowSize.x) / 2.0f, 30.f});

    // 5. Update Coins/Inventory Display (top-right)
    mCoinsText->setString("Coins: " + std::to_string(coins));
    mInventoryText->setString("Potions: " + std::to_string(inventory.getHealthPotionCount()) +
                              " | Grenades: " + std::to_string(inventory.getGrenadeCount()) +
                              " | Magic: " + std::to_string(inventory.getDamagePotionCount()));

    sf::Vector2u winSize = window.getSize();
    sf::FloatRect coinsBounds = mCoinsText->getLocalBounds();
    float coinsX = static_cast<float>(winSize.x) - coinsBounds.size.x - 20.f;
    float coinsY = 10.f;
    mCoinsText->setPosition({coinsX, coinsY});

    sf::FloatRect invBounds = mInventoryText->getLocalBounds();
    float invX = static_cast<float>(winSize.x) - invBounds.size.x - 20.f;
    float invY = coinsY + 28.f;
    mInventoryText->setPosition({invX, invY});

    // 6. Shop text in right-side columns
    std::ostringstream shop;
    shop << std::left;
    shop << "       BUY                          USE\n";
    shop << "1  Grenade (50)           left click  Grenade\n";
    shop << "2  Health  (25)                  P  Health\n";
    shop << "3  Damage  Boost (40)            L  Magic Damage\n";
    shop << "4  Magic Damage  (75)\n";
    mShopText->setString(shop.str());

    sf::FloatRect shopBounds = mShopText->getLocalBounds();
    float shopX = static_cast<float>(winSize.x) - shopBounds.size.x - 20.f;
    float shopY = invY + 28.f;
    mShopText->setPosition({shopX, shopY});
}

void HUD::render(sf::RenderWindow& window) const {
    window.draw(*mPlayerHealthText);
    window.draw(*mTimerText);
    window.draw(*mCoinsText);
    window.draw(*mInventoryText);
    window.draw(*mShopText);

    if (mFlashAlpha > 0.f) {
        window.draw(mDamageOverlay);
    }
}

// Make sure this matches the declaration in HUD.h
void HUD::renderGameOver(sf::RenderWindow& window) {
    sf::FloatRect textBounds = mGameOverText->getLocalBounds();

    // SFML 3 Origin Fix
    mGameOverText->setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    });

    sf::View currentView = window.getView();
    mGameOverText->setPosition(currentView.getCenter());

    window.draw(*mGameOverText);
}

std::ostream& operator<<(std::ostream& os, const HUD& hud) {
    os << "HUD(Font: " << hud.mFontPath << ")";
    return os;
}