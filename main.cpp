#include "header/Game.h"
#include <iostream>
#include "header/GameException.h"

int main() {
    try {
        Game game(1920, 1080, "Shadow of the Cave");
        game.run();
    }
    catch (const AssetLoadException& e) {
        // 1. Handle missing files (Player, Level, Font, etc.)
        std::cerr << "[CRITICAL] " << e.what() << "\n";
        std::cerr << "Make sure the 'assets' folder is next to the executable.\n";
        return -1;
    }
    catch (const GameException& e) {
        // 2. Handle logic errors (like the Grenade cast)
        std::cerr << "[GAME LOGIC ERROR] " << e.what() << "\n";
        return -1;
    }
    catch (const std::exception& e) {
        // 3. Handle anything else
        std::cerr << "[UNKNOWN ERROR] " << e.what() << "\n";
        return -1;
    }

    return 0;
}