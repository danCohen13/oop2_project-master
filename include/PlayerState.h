#pragma once
#include <SFML/Graphics.hpp>

class Player;
class Exhaust; // Forward Declaration (dit au compilateur que ce type existe)

class PlayerState {
public:
    virtual ~PlayerState() = default;

    virtual void update(Player& player, float deltaTime) = 0;

    // Signature unifiée pour le rendu de tous les états
    virtual void draw(sf::RenderWindow& window,
        const sf::Sprite& playerSprite,
        const Exhaust& exhaust) const = 0; // <-- Mis à jour ici
};