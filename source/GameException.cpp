/**
 * @file GameException.cpp
 * @brief Implementation of custom game exception classes
 *
 * Implements exception classes for:
 * - Game configuration errors
 * - Asset loading failures
 * - Invalid entity statistics
 *
 * @see GameException.h
 */

#include "../header/GameException.h"
#include <sstream>


// We prefix it with "Game Error: " to distinguish it from system errors
GameException::GameException(const std::string& message)
    : std::runtime_error("Game Error: " + message)
{
}


// Formats a clear error message and passes it to the base GameException
AssetLoadException::AssetLoadException(const std::string& assetType, const std::string& filepath)
    : GameException("Failed to load " + assetType + " from: " + filepath)
{
}

GameConfigException::GameConfigException(const std::string& component, const std::string& reason)
    : GameException("Configuration Error in [" + component + "]: " + reason) {}

InvalidStatException::InvalidStatException(const std::string& context, const std::string& statName, const float value)
    : GameException([&] {
        std::ostringstream oss;
        oss << "Invalid Stat in " << context << " -> " << statName << " cannot be " << value;
        return oss.str();
    }()) {}
