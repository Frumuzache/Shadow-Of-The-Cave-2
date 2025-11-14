#include "../header/HUD.h"
#include <stdexcept>
#include <string>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

HUD::HUD(std::string fontPath)
: mFontPath(std::move(fontPath)),
  mFont(),
  mPlayerHealthText(mFont)
{

    if (!mFont.openFromFile(mFontPath))
        throw std::runtime_error("Failed to load font: " + mFontPath);

    mPlayerHealthText.setCharacterSize(24);
    mPlayerHealthText.setFillColor(sf::Color::White);
    mPlayerHealthText.setPosition({10.f, 10.f});
}

// Update HUD elements based on game state
void HUD::update(const Player& player) {
    mPlayerHealthText.setString("Health: " + std::to_string(static_cast<float>(player.getCurrentHealth())));
}

void HUD::render(sf::RenderWindow& window) const {
    window.draw(mPlayerHealthText);
}

std::ostream& operator<<(std::ostream& os, const HUD& hud) {
    os << "HUD(Font: " << hud.mFontPath << ")";
    return os;
}