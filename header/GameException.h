/**
 * @file GameException.h
 * @brief Custom exception classes for game error handling
 */

#pragma once
#include <stdexcept>
#include <string>

/**
 * @class GameException
 * @brief Base exception class for all game-specific errors
 *
 * Derives from std::runtime_error and provides a base for more specific exceptions.
 */
class GameException : public std::runtime_error {
public:
    /**
     * @brief Constructor
     * @param message Error message describing the exception
     */
    explicit GameException(const std::string& message);
};

/**
 * @class AssetLoadException
 * @brief Exception thrown when game assets (textures, fonts) fail to load
 *
 * Indicates missing or corrupted asset files.
 */
class AssetLoadException : public GameException {
public:
    /**
     * @brief Constructor
     * @param assetType Type of asset that failed to load (e.g., "Texture", "Font")
     * @param filepath Path to the asset file that failed
     */
    AssetLoadException(const std::string& assetType, const std::string& filepath);
};

/**
 * @class GameConfigException
 * @brief Exception thrown when game initialization fails due to bad parameters
 *
 * Used when Game constructor receives invalid configuration.
 */
class GameConfigException : public GameException {
public:
    /**
     * @brief Constructor
     * @param component Component that failed to initialize (e.g., "Window", "Level")
     * @param reason Reason for the configuration error
     */
    explicit GameConfigException(const std::string& component, const std::string& reason);
};

/**
 * @class InvalidStatException
 * @brief Exception thrown when an object is created with invalid statistics
 *
 * Used when entity stats violate game rules (negative health, etc.).
 */
class InvalidStatException : public GameException {
public:
    /**
     * @brief Constructor
     * @param context Context where the error occurred (e.g., "Player", "Enemy")
     * @param statName Name of the invalid stat (e.g., "health", "damage")
     * @param value The invalid value provided
     */
    InvalidStatException(const std::string& context, const std::string& statName, float value);
};