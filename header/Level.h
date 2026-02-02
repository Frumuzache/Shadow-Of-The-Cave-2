/**
 * @file Level.h
 * @brief Game level/background class
 */

#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <string>

/**
 * @class Level
 * @brief Represents the game level with background texture
 *
 * Manages background sprite rendering and level boundaries.
 * Currently handles a single static background image.
 */
class Level {
public:
    /**
     * @brief Constructor
     * @param texturePath Path to background texture image
     * @param mapBoundries Game world boundaries (size)
     * @throws AssetLoadException if texture cannot be loaded
     */
    explicit Level(std::string texturePath, sf::Vector2f mapBoundries);

    /**
     * @brief Render level background
     * @param window Target render window
     */
    void render(sf::RenderWindow& window) const;

    /// @brief Output stream operator for debugging
    friend std::ostream& operator<<(std::ostream& os, const Level& level);

private:
    /// @name Level Assets
    /// @{
    std::string mTexturePath;        ///< Path to background texture
    sf::Texture mBackgroundTexture;  ///< Background texture resource
    sf::Sprite mBackgroundSprite;    ///< Background sprite for rendering
    /// @}
};

#endif //LEVEL_H