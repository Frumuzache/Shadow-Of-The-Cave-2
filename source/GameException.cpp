#include "../header/GameException.h"


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

