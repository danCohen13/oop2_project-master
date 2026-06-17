#include "WalkState.h"
#include "Player.h"
#include "Resources.h"

WalkState::WalkState() : m_currentFrame(0), m_frameTimer(0.0f) {}

void WalkState::update(Player& player, float deltaTime) {
    auto& sprite = player.getSprite();
    const sf::Texture& normalTex = Resources::getInstance().getTexture("player");

    // CORRECTION SFML 3.0 : Utilisation de '&' pour valider la comparaison d'adresses
    if (&sprite.getTexture() != &normalTex) {
        sprite.setTexture(normalTex, false);
        sprite.setScale({ 1.f, 1.f });        // Remet à l'endroit
        sprite.setRotation(sf::degrees(0.f)); // Remise droite (SFML 3.0)
        sprite.setOrigin({ 0.f, 0.f });       // FIX ANCRAGE EXHAUST : Repère en haut à gauche
    }

    m_frameTimer += deltaTime;
    if (m_frameTimer >= FRAME_DURATION) {
        m_frameTimer = 0.0f;
        m_currentFrame = (m_currentFrame + 1) % 4; // Boucle sur les 4 frames de course

        int playerWidth = normalTex.getSize().x / 4;
        int playerHeight = normalTex.getSize().y;

        sprite.setTextureRect(sf::IntRect({ static_cast<int>(m_currentFrame * playerWidth), 0 }, { playerWidth, playerHeight }));
    }
}

void WalkState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust& exhaust) const {
    exhaust.draw(window);      // N'affichera rien car m_active sera false
    window.draw(playerSprite);
}