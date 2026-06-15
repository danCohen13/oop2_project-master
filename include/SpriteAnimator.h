#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Responsabilité unique : découper une spritesheet et avancer frame par frame.
// Utilisé PAR COMPOSITION dans StaticGameObject, WalkState, JumpState et Exhaust.
class SpriteAnimator {
public:
    // texture       : la spritesheet chargée
    // frameCount    : nombre de frames côte à côte horizontalement
    // frameDuration : secondes par frame (0.1f par défaut)
    SpriteAnimator(const sf::Texture& texture, int frameCount, float frameDuration = 0.1f);

    // Avance le timer. Retourne true si la frame a changé ce tick.
    bool update(float deltaTime);

    // Applique le bon textureRect sur le sprite cible.
    void applyTo(sf::Sprite& sprite) const;

    // Remet le compteur à la frame 0 (utile lors d'une transition d'état).
    void reset();

    // Taille d'une frame individuelle (largeur × hauteur en pixels).
    sf::Vector2i getFrameSize() const;

    int getCurrentFrame() const { return static_cast<int>(m_currentFrame); }

private:
    std::vector<sf::IntRect> m_frames;
    size_t m_currentFrame = 0;
    float  m_frameTimer = 0.f;
    float  m_frameDuration;
};