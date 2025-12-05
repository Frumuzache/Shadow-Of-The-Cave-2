#include "../header/HUD.h"
#include <stdexcept>
#include <string>
#include <sstream> // Required for string streams
#include <iomanip> // Required for std::setw and std::setfill
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

HUD::HUD(std::string fontPath)
: mFontPath(std::move(fontPath)),
  mFont(),
  mPlayerHealthText(mFont),
  mTimerText(mFont)
{
    if (!mFont.openFromFile(mFontPath))
        throw std::runtime_error("Failed to load font: " + mFontPath);

    mPlayerHealthText.setCharacterSize(24);
    mPlayerHealthText.setFillColor(sf::Color::White);
    mPlayerHealthText.setPosition({10.f, 10.f});

    mTimerText.setCharacterSize(24);
    mTimerText.setFillColor(sf::Color::White);
}

// Update HUD elements based on game state
// Note: We use 'const sf::RenderWindow&' to fix the warning
void HUD::update(const Player& player, const sf::RenderWindow& window) {
    // 1. Update Health
    mPlayerHealthText.setString("Health: " + std::to_string(static_cast<float>(player.getCurrentHealth())));

    // 2. Update Timer
    sf::Time elapsed = mLevelClock.getElapsedTime();
    int totalSeconds = static_cast<int>(elapsed.asSeconds());
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    // Format string as MM:SS (e.g., 05:09)
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;

    mTimerText.setString(ss.str());

    // 3. Center the Timer at the top of the screen
    sf::Vector2u windowSize = window.getSize();
    sf::FloatRect textBounds = mTimerText.getLocalBounds();

    // --- FIX FOR SFML 3 ---
    // Use position.x/y instead of left/top
    // Use size.x/y instead of width/height
    mTimerText.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    });

    // Set position to middle of screen width, and 30 pixels down from top
    mTimerText.setPosition({static_cast<float>(windowSize.x) / 2.0f, 30.f});
}

void HUD::render(sf::RenderWindow& window) const {
    window.draw(mPlayerHealthText);
    window.draw(mTimerText); // Don't forget to draw the timer!
}

std::ostream& operator<<(std::ostream& os, const HUD& hud) {
    os << "HUD(Font: " << hud.mFontPath << ")";
    return os;
}