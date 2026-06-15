#pragma once
#include <stdexcept>
#include <string>

// Classe personnalisée pour les erreurs critiques du jeu
class GameException : public std::runtime_error {
public:
    // Le constructeur prend le message d'erreur et le passe à la classe parente
    explicit GameException(const std::string& message)
        : std::runtime_error(message) {}
};