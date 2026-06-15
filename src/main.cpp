#include "Controller.h"
#include "GameException.h"
#include <iostream>
#include <cstdlib> // Pour EXIT_SUCCESS et EXIT_FAILURE

int main() {

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    try {
        // Le jeu essaie de se lancer normalement
        Controller game;
        game.run();
    }
    catch (const GameException& e) {
        // Si l'une de VOS exceptions est levée (ex: image manquante)
        std::cerr << "\n[ERREUR DU JEU] " << e.what() << "\n\n";
        return EXIT_FAILURE;
    }
    catch (const std::exception& e) {
        // Si une exception C++ standard survient (ex: manque de mémoire RAM)
        std::cerr << "\n[ERREUR STANDARD] " << e.what() << "\n\n";
        return EXIT_FAILURE;
    }
    catch (...) {
        // Le filet de sécurité ultime pour les erreurs inconnues
        std::cerr << "\n[ERREUR FATALE] Une erreur inconnue est survenue.\n\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}