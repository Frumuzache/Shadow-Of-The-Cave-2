/**
 * @file Weapon.h
 * @brief Base weapon class and weapon type enumeration
 *
 * Defines the abstract Weapon class and WeaponType enum for all game weapons.
 */

#ifndef WEAPON_H
#define WEAPON_H
#include <SFML/Graphics.hpp>

#include <string>

/**
 * @enum WeaponType
 * @brief Enumeration of weapon types in the game
 */
enum class WeaponType {
    Melee,       ///< Close-range melee weapon (sword, claws)
    Ranged,      ///< Long-range ranged weapon (rifle, bow)
    Magic,       ///< Magic spell weapon
    Throwable    ///< Throwable weapon (grenades)
};

/**
 * @class Weapon
 * @brief Abstract base class for all weapon types
 *
 * Defines common weapon properties and behaviors:
 * - Damage and reload time
 * - Range and attack type
 * - Rendering and positioning
 *
 * Derived classes: MeleeWeapon, RangedWeapon, ThrowableWeapon, MagicWeapon
 */
class Weapon {
public:
    /// @brief Default constructor
    Weapon();

    /**
     * @brief Parameterized constructor
     * @param name Weapon name
     * @param damage Damage per hit
     * @param reloadTime Cooldown between attacks
     * @param range Attack range in pixels
     * @param type Weapon type (default: Ranged)
     */
    Weapon(std::string name, float damage, float reloadTime, float range, WeaponType type = WeaponType::Ranged);

    /**
     * @brief Load weapon texture from file
     * @param path Path to texture image file
     * @throws AssetLoadException if texture cannot be loaded
     */
    void loadTexture(const std::string& path);

    /**
     * @brief Update weapon position and rotation
     * @param playerPos Player's current position
     * @param rotationAngle Rotation angle in degrees
     */
    void update(sf::Vector2f playerPos, sf::Angle rotationAngle);

    /**
     * @brief Render weapon sprite to window
     * @param window Target render window
     */
    virtual void render(sf::RenderWindow& window) const;

    /// @name Constructors and Assignment (Rule of Five)
    /// @{
    /// @brief Virtual destructor for proper cleanup
    virtual ~Weapon();

    /**
     * @brief Copy constructor
     * @param other Source weapon to copy
     */
    Weapon(const Weapon& other);

    /**
     * @brief Copy assignment operator
     * @param other Source weapon to copy
     * @return Reference to this weapon
     */
    Weapon& operator=(const Weapon& other);
    /// @}

    /// @name Property Getters
    /// @{
    /// @brief Get weapon damage value
    float getDamage() const;

    /// @brief Get reload time between attacks
    float getReloadTime() const;

    /// @brief Get weapon type
    WeaponType getType() const;

    /// @brief Get attack range in pixels
    float getRange() const;

    /// @brief Get weapon name
    const std::string& getName() const;
    /// @}

    /// @name Property Setters
    /// @{
    /**
     * @brief Set weapon damage
     * @param newDamage New damage value
     */
    void setDamage(float newDamage);

    /**
     * @brief Set visual size of weapon sprite
     * @param width Width in pixels
     * @param height Height in pixels
     */
    void setVisualSize(float width, float height);

    /**
     * @brief Set sprite scale
     * @param scale Scale multiplier
     */
    void setScale(float scale);
    /// @}

    /**
     * @brief Create a clone of this weapon
     * @return Unique pointer to a copy of this weapon
     */
    virtual std::unique_ptr<Weapon> clone() const = 0;

    /// @brief Output stream operator for debugging
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon);

    /// @brief Print weapon information to stream
    virtual void print(std::ostream& os) const;

    /// @brief Apply damage upgrade to weapon
    virtual void applyUpgrade() = 0;

protected:
    /// @name Protected Members - Rendering
    /// @{
    sf::Texture mTexture;        ///< Weapon sprite texture
    sf::Sprite mSprite;          ///< Weapon sprite for rendering
    /// @}

    /// @name Protected Members - Properties
    /// @{
    WeaponType type;             ///< Type of weapon
    std::string name;            ///< Weapon name
    float damage;                ///< Damage per hit
    float reloadTime;            ///< Cooldown between attacks (seconds)
    float range;                 ///< Attack range (pixels)
    /// @}
};


#endif //WEAPON_H