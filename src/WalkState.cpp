#include "WalkState.h"
#include "Player.h"
#include "Resources.h"

WalkState::WalkState() : m_currentFrame(0), m_frameTimer(0.0f) {}

void WalkState::update(Player& player, float deltaTime) {
    m_frameTimer += deltaTime;
    if (m_frameTimer >= FRAME_DURATION) {
        m_frameTimer = 0.0f;
        m_currentFrame = (m_currentFrame + 1) % 4; // Boucle sur les 4 frames de course

        const sf::Texture& playerTex = Resources::getInstance().getTexture("player");
        int playerWidth = playerTex.getSize().x / 4;
        int playerHeight = playerTex.getSize().y;

        player.getSprite().setTextureRect(sf::IntRect({ m_currentFrame * playerWidth, 0 }, { playerWidth, playerHeight }));
    }
}

void WalkState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust& exhaust) const {
    exhaust.draw(window);      // N'affichera rien car m_active sera false
    window.draw(playerSprite);
}