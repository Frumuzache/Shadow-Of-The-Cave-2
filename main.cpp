#include "header/Game.h"
#include <iostream>
#include <cstdlib>

int main()
{
    try {
        Game game(1920, 1080, "Shadow Of The Cave");
        game.run();
    }
    catch (const std::exception& e) {
        std::cerr << "An unrecoverable error occurred: " << e.what() << std::endl;
        return EXIT_FAILURE; // Exit the program with an error code
    }

    return EXIT_SUCCESS;
}