#include "DeadState.h"
#include "Player.h"
#include "Resources.h"

DeadState::DeadState()
    : m_animator(
        Resources::getInstance().getTexture("PlayerDead"),
        static_cast<int>(Resources::getInstance().getTexture("PlayerDead").getSize().x /
            Resources::getInstance().getTexture("PlayerDead").getSize().y),
        0.07f
    ),
    m_finished(false),
    m_started(false)
{
}

void DeadState::update(Player& player, float deltaTime) {
    // Appliquer la physique de chute pour que le corps tombe naturellement au sol
    player.applyStandardPhysics(deltaTime, false);

    auto& sprite = player.getSprite();
    const sf::Texture& deadTex = Resources::getInstance().getTexture("PlayerDead");

    if (&sprite.getTexture() != &deadTex) {
        sprite.setTexture(deadTex, false);

        int totalFrames = static_cast<int>(deadTex.getSize().x / deadTex.getSize().y);
        int fw = static_cast<int>(deadTex.getSize().x) / totalFrames;
        int fh = static_cast<int>(deadTex.getSize().y);
        sprite.setTextureRect(sf::IntRect({ 0, 0 }, { fw, fh }));
    }

    player.getExhaust().setActive(false);

    if (!m_finished) {
        bool frameChanged = m_animator.update(deltaTime);
        m_animator.applyTo(sprite);

        if (frameChanged) {
            m_started = true;
        }

        if (m_started && m_animator.getCurrentFrame() == 0) {
            m_finished = true;

            int totalFrames = static_cast<int>(deadTex.getSize().x / deadTex.getSize().y);
            int fw = static_cast<int>(deadTex.getSize().x) / totalFrames;
            int fh = static_cast<int>(deadTex.getSize().y);

            sprite.setTextureRect(sf::IntRect({ (totalFrames - 1) * fw, 0 }, { fw, fh }));
        }
    }
}

void DeadState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust&) const {
    window.draw(playerSprite);
}