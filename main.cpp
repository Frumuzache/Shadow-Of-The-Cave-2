#include "header/Game.h"
#include <iostream>     // For std::cerr
#include <cstdlib>      // For EXIT_SUCCESS and EXIT_FAILURE

int main()
{
    try {
        // Create the game object using the parameterized constructor
        // This assumes you want a 1920x1080 window. You can change these values.
        Game game(1920, 1080, "Shadow Of The Cave");

        // Start the main game loop
        game.run();
    }
    catch (const std::exception& e) {
        // If the game constructor or run() throws an error (e.g., "Failed to load..."),
        // we catch it here and report it to the console.
        std::cerr << "An unrecoverable error occurred: " << e.what() << std::endl;
        return EXIT_FAILURE; // Exit the program with an error code
    }

    // If the game loop exits normally
    return EXIT_SUCCESS;
}