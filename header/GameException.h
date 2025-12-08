#ifndef GAME_EXCEPTION_H
#define GAME_EXCEPTION_H

#include <stdexcept>
#include <string>

// 1. Base Game Exception
// Inherits from std::runtime_error so it can be caught by standard catches
class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message);
};

// 2. Specific Exception for Missing Files
// Inherits from GameException for specific asset handling
class AssetLoadException : public GameException {
public:
    // Takes asset type (e.g., "Texture") and path (e.g., "assets/player.png")
    AssetLoadException(const std::string& assetType, const std::string& filepath);
};

#endif