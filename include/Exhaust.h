#pragma once
#include <SFML/Graphics.hpp>
#include "SpriteAnimator.h"

// Responsabilité unique : gérer le sprite, l'animation et le positionnement
// de la flamme du jetpack. Player et JumpState délèguent tout ce qui concerne
// la flamme à cette classe.
class Exhaust {
public:
    Exhaust();

    // Appelé chaque frame quand le jetpack est actif.
    // playerPos = coin supérieur gauche du sprite joueur.
    void update(float deltaTime, const sf::Vector2f& playerPos);

    // Dessine la flamme. Ne rien appeler si isActive() est faux.
    void draw(sf::RenderWindow& window) const;

    bool isActive() const { return m_active; }

    // Active ou désactive la flamme (appelé par JumpState).
    void setActive(bool active);

    // Synchronise la frame de la flamme avec celle du joueur
    // (les deux animations bougent ensemble).
    void syncFrame(int playerFrame);

private:
    sf::Sprite     m_sprite;
    SpriteAnimator m_animator;
    bool           m_active = false;

    // Décalages fixes par rapport au coin supérieur gauche du sprite joueur.
    static constexpr float OFFSET_X = 22.f;
    static constexpr float OFFSET_Y = 80.f;
};