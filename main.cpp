/**
 * @file main.cpp
 * @brief Entry point for Shadow of the Cave 2 game
 *
 * This file contains the main function which initializes the game,
 * handles all exceptions, and runs the main game loop.
 */

#include "header/Game.h"
#include <iostream>
#include "header/GameException.h"

/**
 * @brief Main entry point for the game
 *
 * Creates a game instance with configured window size and title,
 * then runs the game loop. Implements comprehensive error handling
 * for various exception types that may occur during gameplay.
 *
 * Exit codes:
 * - 0: Game exited normally
 * - 1: Critical configuration error
 * - 2: Asset/file loading error
 * - 3: Data integrity error (invalid stats)
 * - 4: Generic game error
 * - -1: Unexpected system error
 *
 * @return Exit code indicating how the game terminated
 */
int main() {
    try {
        /// Create and run the game with 1920x1080 resolution
        Game game(1920, 1080, "Shadow of the Cave");
        game.run();
    }
    /// @name Exception Handling
    /// @{

    /// Catches errors from invalid game configuration
    catch (const GameConfigException& e) {
        std::cerr << "\n[CRITICAL CONFIG ERROR]\n";
        std::cerr << "The game could not start because: " << e.what() << "\n";
        std::cerr << "Please check your code in main.cpp parameters.\n";
        return 1;
    }

    /// Catches errors related to missing files (textures, fonts, etc.)
    catch (const AssetLoadException& e) {
        std::cerr << "\n[ASSET MISSING]\n";
        std::cerr << e.what() << "\n";
        std::cerr << "Ensure the 'assets' folder is next to the executable.\n";
        return 2;
    }

    /// Catches errors when entities are created with invalid statistics
    catch (const InvalidStatException& e) {
        std::cerr << "\n[DATA INTEGRITY ERROR]\n";
        std::cerr << "A game object was created with broken stats:\n";
        std::cerr << e.what() << "\n";
        return 3;
    }

    /// Catches any other game-specific exceptions
    catch (const GameException& e) {
        std::cerr << "\n[GENERIC GAME ERROR]: " << e.what() << "\n";
        return 4;
    }

    /// Safety net for standard C++ errors (std::out_of_range, std::bad_alloc, etc.)
    catch (const std::exception& e) {
        std::cerr << "\n[UNEXPECTED SYSTEM ERROR]: " << e.what() << "\n";
        return -1;
    }
    /// @}

    return 0;
}