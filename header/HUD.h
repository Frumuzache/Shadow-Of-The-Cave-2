/**
 * @file HUD.h
 * @brief Head-Up Display for showing game information
 */

#pragma once
#include "Player.h"
#include "Inventory.h"
#include <SFML/Graphics.hpp>

/**
 * @class HUD
 * @brief Manages on-screen display of game information
 *
 * Displays:
 * - Player health status
 * - Coins/currency
 * - Inventory items
 * - Shop information and keybinds
 * - Elapsed survival time
 * - Game over screen
 */
class HUD {
public:
    /**
     * @brief Constructor
     * @param fontPath Path to font file for text rendering
     * @throws AssetLoadException if font cannot be loaded
     */
    explicit HUD(std::string fontPath);

    /// @brief Destructor to clean up dynamically allocated Text objects
    ~HUD();

    /// @brief Deleted copy constructor (rule of five)
    HUD(const HUD&) = delete;

    /// @brief Deleted copy assignment operator (rule of five)
    HUD& operator=(const HUD&) = delete;

    /**
     * @brief Update HUD information
     * @param player Reference to player for health and stats
     * @param window Reference to render window for dimensions
     * @param deltaTime Time elapsed since last frame (for animations)
     * @param coins Current coin count
     * @param inventory Player's inventory
     */
    void update(const Player& player, const sf::RenderWindow& window, sf::Time deltaTime, int coins, const Inventory& inventory);

    /**
     * @brief Render HUD to window
     * @param window Target render window
     */
    void render(sf::RenderWindow& window) const;

    /**
     * @brief Render game over screen
     * @param window Target render window
     */
    void renderGameOver(sf::RenderWindow& window);

    /// @brief Output stream operator for debugging
    friend std::ostream& operator<<(std::ostream& os, const HUD& hud);

private:
    /// @name Font Resource
    /// @{
    std::string mFontPath;           ///< Path to font file
    sf::Font mFont;                  ///< Font for text rendering
    /// @}

    /// @name HUD Text Elements
    /// @{
    sf::Text* mCoinsText = nullptr;      ///< Display coins/currency
    sf::Text* mInventoryText = nullptr;  ///< Display inventory items count
    sf::Text* mShopText = nullptr;       ///< Display shop keybinds and prices

    sf::Text* mPlayerHealthText = nullptr;  ///< Display player health
    sf::Text* mTimerText = nullptr;      ///< Display survival time
    sf::Text* mGameOverText = nullptr;   ///< Game over message
    /// @}

    /// @name Timers
    /// @{
    sf::Clock mLevelClock;           ///< Survival time tracker
    /// @}

    /// @name Visual Effects
    /// @{
    sf::RectangleShape mDamageOverlay;  ///< Red overlay for damage feedback
    float mFlashAlpha;               ///< Alpha for damage flash effect
    float mPreviousHealth;           ///< Previous health for damage detection
    /// @}
};