/**
 * @file Level.cpp
 * @brief Implementation of the Level class
 *
 * Implements level management including:
 * - Background texture loading
 * - Level rendering
 *
 * @see Level.h
 */

#include "../header/Level.h"
#include <stdexcept>
#include <utility>
#include "../header/GameException.h"

Level::Level(std::string  texturePath, sf::Vector2f mapBoundries)
    : mTexturePath(std::move(texturePath)),
        mBackgroundTexture{},
        mBackgroundSprite(mBackgroundTexture)
{
    if (!mBackgroundTexture.loadFromFile(mTexturePath))
        throw AssetLoadException("Level Background", mTexturePath);

    // mBackgroundSprite.setTexture(mBackgroundTexture, true);

    mBackgroundTexture.setRepeated(true);
    mBackgroundSprite.setTexture(mBackgroundTexture);

    // Use the passed mapSize instead of hardcoded 3000
    mBackgroundSprite.setTextureRect(sf::IntRect(
        {0, 0},
        {static_cast<int>(mapBoundries.x), static_cast<int>(mapBoundries.y)}
    ));
}

void Level::render(sf::RenderWindow& window) const {
    window.draw(mBackgroundSprite);
}

std::ostream& operator<<(std::ostream& os, const Level& level) {
    os << "Level(Texture: " << level.mTexturePath << ")";
    return os;
}