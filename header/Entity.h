/**
 * @file Entity.h
 * @brief Base abstract class for all game entities
 *
 * Provides common interface for Player and Enemy classes.
 * Manages position, health, movement speed, and sprite rendering.
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

/**
 * @class Entity
 * @brief Abstract base class for all game entities (Player and Enemies)
 *
 * Defines common properties and behaviors:
 * - Position and sprite rendering
 * - Health management
 * - Movement speed
 * - Collision bounds
 *
 * Derived classes: Player, Enemy, MeleeEnemy, RangedEnemy
 */
class Entity {
public:
    /**
     * @brief Default constructor
     * Initializes entity with default values
     */
    Entity();

    /**
     * @brief Parameterized constructor
     * @param speed Movement speed of the entity
     * @param health Maximum and current health of the entity
     */
    Entity(float speed, float health);

    /// @brief Virtual destructor for proper cleanup of derived classes
    virtual ~Entity() = default;

    /**
     * @brief Update entity state
     * @param deltaTime Time elapsed since last frame
     * @param mapBounds Game world boundaries for boundary checking
     * @param window Reference to render window for input handling
     */
    virtual void update(sf::Time deltaTime, const sf::Vector2f& mapBounds, const sf::RenderWindow& window) = 0;

    /**
     * @brief Render entity to window
     * @param window Target render window
     */
    virtual void render(sf::RenderWindow& window) const = 0;

    /// @name Position and Bounds
    /// @{
    /**
     * @brief Get the global bounding box of the entity
     * @return SFML FloatRect representing the entity's bounds
     */
    sf::FloatRect getGlobalBounds() const;

    /**
     * @brief Get current position of entity
     * @return Vector2f representing position (x, y)
     */
    sf::Vector2f getPosition() const;

    /**
     * @brief Set position of entity
     * @param position New position vector
     * @return The position that was set
     */
    sf::Vector2f setPosition(const sf::Vector2f& position);

    /**
     * @brief Get size of the entity's sprite texture
     * @return Vector2u representing width and height in pixels
     */
    sf::Vector2u getSpriteSize() const;
    /// @}

    /// @name Health Management
    /// @{
    /**
     * @brief Get current health of entity
     * @return Current health value
     */
    virtual float getCurrentHealth() const;

    /**
     * @brief Apply damage to entity
     * @param damage Amount of damage to apply (reduces current health)
     */
    virtual void takeDamage(float damage);
    /// @}

    /// @brief Output stream operator for debugging
    friend std::ostream& operator<<(std::ostream& os, const Entity& entity);

protected:
    /// @name Protected Members - Rendering
    /// @{
    sf::Texture mTexture;        ///< Entity sprite texture
    sf::Sprite mSprite;          ///< Entity sprite for rendering
    /// @}

    /// @name Protected Members - Physics
    /// @{
    float mMovementSpeed;        ///< Speed of movement (pixels/second)
    /// @}

    /// @name Protected Members - Health
    /// @{
    float maxHealth;             ///< Maximum health value
    float currentHealth;         ///< Current health value
    /// @}
};

#endif //ENTITY_H