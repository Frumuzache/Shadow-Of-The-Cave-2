#include "../header/Level.h"
#include <stdexcept>
#include <utility>

Level::Level(std::string  texturePath)
    : mTexturePath(std::move(texturePath)),
        mBackgroundTexture{},
        mBackgroundSprite(mBackgroundTexture)
{
    if (!mBackgroundTexture.loadFromFile(mTexturePath))
        throw std::runtime_error("Failed to load texture: " + mTexturePath);

    mBackgroundSprite.setTexture(mBackgroundTexture, true);
}

void Level::render(sf::RenderWindow& window) const {
    window.draw(mBackgroundSprite);
}

std::ostream& operator<<(std::ostream& os, const Level& level) {
    os << "Level(Texture: " << level.mTexturePath << ")";
    return os;
}