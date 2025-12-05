#include "../header/HUD.h"
#include <stdexcept>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint> // <--- REQUIRED for std::uint8_t in SFML 3

HUD::HUD(std::string fontPath)
: mFontPath(std::move(fontPath)),
  mFont(),
  // INITIALIZATION MUST MATCH HEADER ORDER
  mPlayerHealthText(mFont),
  mTimerText(mFont),
  mGameOverText(mFont),
  mLevelClock(),
  mDamageOverlay(),
  mFlashAlpha(0.f),
  mPreviousHealth(100.f)
{
    if (!mFont.openFromFile(mFontPath))
        throw std::runtime_error("Failed to load font: " + mFontPath);

    // Setup Health
    mPlayerHealthText.setCharacterSize(24);
    mPlayerHealthText.setFillColor(sf::Color::White);
    mPlayerHealthText.setPosition({10.f, 10.f});

    // Setup Timer
    mTimerText.setCharacterSize(24);
    mTimerText.setFillColor(sf::Color::White);

    // Setup Game Over
    mGameOverText.setString("ZERO COMUNICARE DOMU STUDENT");
    mGameOverText.setCharacterSize(72);
    mGameOverText.setFillColor(sf::Color::Red);
    mGameOverText.setStyle(sf::Text::Bold);

    // Setup Damage Flash
    mDamageOverlay.setSize({4000.f, 4000.f});
    mDamageOverlay.setFillColor(sf::Color(255, 0, 0, 0));
}

void HUD::update(const Player& player, const sf::RenderWindow& window, sf::Time deltaTime) {
    // 1. Update Health
    float currentHealth = player.getCurrentHealth();
    mPlayerHealthText.setString("Health: " + std::to_string(static_cast<int>(currentHealth)));

    // 2. Damage Flash Logic
    if (currentHealth < mPreviousHealth) {
        mFlashAlpha = 150.f; // Start flash
    }
    mPreviousHealth = currentHealth;

    if (mFlashAlpha > 0.f) {
        mFlashAlpha -= 300.f * deltaTime.asSeconds();
        if (mFlashAlpha < 0.f) mFlashAlpha = 0.f;

        // FIX: Use std::uint8_t instead of sf::Uint8
        mDamageOverlay.setFillColor(sf::Color(255, 0, 0, static_cast<std::uint8_t>(mFlashAlpha)));

        // Keep overlay centered on camera
        // --- CHANGED LOGIC HERE ---
        // Instead of calculating the camera center, we simply fill the window.
        // Because HUD is rendered in "Default View" (Screen Space), 0,0 is always top-left.

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

    mTimerText.setString(ss.str());

    // 4. Center Timer (SFML 3 Fix)
    sf::Vector2u windowSize = window.getSize();
    sf::FloatRect textBounds = mTimerText.getLocalBounds();

    mTimerText.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    });

    mTimerText.setPosition({static_cast<float>(windowSize.x) / 2.0f, 30.f});
}

void HUD::render(sf::RenderWindow& window) const {
    window.draw(mPlayerHealthText);
    window.draw(mTimerText);

    if (mFlashAlpha > 0.f) {
        window.draw(mDamageOverlay);
    }
}

// Make sure this matches the declaration in HUD.h
void HUD::renderGameOver(sf::RenderWindow& window) {
    sf::FloatRect textBounds = mGameOverText.getLocalBounds();

    // SFML 3 Origin Fix
    mGameOverText.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    });

    sf::View currentView = window.getView();
    mGameOverText.setPosition(currentView.getCenter());

    window.draw(mGameOverText);
}

std::ostream& operator<<(std::ostream& os, const HUD& hud) {
    os << "HUD(Font: " << hud.mFontPath << ")";
    return os;
}