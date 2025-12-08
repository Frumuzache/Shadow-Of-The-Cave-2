#include "header/Game.h"
#include <iostream>
#include "header/GameException.h"

int main() {
    try {
        Game game(1920, 1080, "Shadow of the Cave");
        game.run();
    }
    catch (const GameConfigException& e) {
        std::cerr << "\n[CRITICAL CONFIG ERROR]\n";
        std::cerr << "The game could not start because: " << e.what() << "\n";
        std::cerr << "Please check your code in main.cpp parameters.\n";
        return 1;
    }

    // Catches errors related to missing files
    catch (const AssetLoadException& e) {
        std::cerr << "\n[ASSET MISSING]\n";
        std::cerr << e.what() << "\n";
        std::cerr << "Ensure the 'assets' folder is next to the executable.\n";
        return 2;
    }

    catch (const InvalidStatException& e) {
        std::cerr << "\n[DATA INTEGRITY ERROR]\n";
        std::cerr << "A game object was created with broken stats:\n";
        std::cerr << e.what() << "\n";
        return 3;
    }

    catch (const GameException& e) {
        std::cerr << "\n[GENERIC GAME ERROR]: " << e.what() << "\n";
        return 4;
    }

    // Safety net for standard C++ errors (std::out_of_range, std::bad_alloc, etc.)
    catch (const std::exception& e) {
        std::cerr << "\n[UNEXPECTED SYSTEM ERROR]: " << e.what() << "\n";
        return -1;
    }

    return 0;
}