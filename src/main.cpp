#include "Controller.h"
#include "GameException.h"
#include <iostream>
#include <cstdlib> 

int main() {

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    try {
        Controller game;
        game.run();
    }
    catch (const GameException& e) {
        std::cerr << "\n[ERROR OF THE GAME] " << e.what() << "\n\n";
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        std::cerr << "\n[STANDARD ERROR] " << e.what() << "\n\n";
        return EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "\n[FATAL ERROR] Unkown error happened.\n\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}