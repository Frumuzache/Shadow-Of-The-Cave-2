#include "../header/Level.h"
#include <stdexcept>
#include <utility>

Level::Level(std::string  texturePath, sf::Vector2f mapBoundries)
    : mTexturePath(std::move(texturePath)),
        mBackgroundTexture{},
        mBackgroundSprite(mBackgroundTexture)
{
    if (!mBackgroundTexture.loadFromFile(mTexturePath))
        throw std::runtime_error("Failed to load texture: " + mTexturePath);

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