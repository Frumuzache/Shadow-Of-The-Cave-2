#pragma once
#include <stdexcept>
#include <string>

// 1. Base Class
class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message);
};

// 2. RESOURCE ERROR
class AssetLoadException : public GameException {
public:
    AssetLoadException(const std::string& assetType, const std::string& filepath);
};

// Used when the Game fails to start due to bad parameters
class GameConfigException : public GameException {
public:
    explicit GameConfigException(const std::string& component, const std::string& reason);
};

// Used when an object is created with stats that break game rules
class InvalidStatException : public GameException {
public:
    InvalidStatException(const std::string& context, const std::string& statName, float value);
};