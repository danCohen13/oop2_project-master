#include "DeadState.h"
#include "Player.h"
#include "Resources.h"

DeadState::DeadState()
    : m_animator(
        Resources::getInstance().getTexture("PlayerDead"),
        sf::Vector2i(Resources::getInstance().getTexture("PlayerDead").getSize()).x /
        sf::Vector2i(Resources::getInstance().getTexture("PlayerDead").getSize()).y,
        0.07f
    ),
    m_finished(false),
    m_started(false)
{
}

void DeadState::update(Player& player, float deltaTime) {
    player.applyStandardPhysics(deltaTime, false);

    auto& sprite = player.getSprite();
    const sf::Texture& deadTex = Resources::getInstance().getTexture("PlayerDead");

    if (&sprite.getTexture() != &deadTex) {
        sprite.setTexture(deadTex, false);

        sf::Vector2i frameSize = m_animator.getFrameSize();
        sprite.setTextureRect(sf::IntRect({ 0, 0 }, frameSize));
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

            sf::Vector2i texSize(deadTex.getSize());
            sf::Vector2i frameSize = m_animator.getFrameSize();
            int totalFrames = texSize.x / frameSize.x; 

            sprite.setTextureRect(sf::IntRect({ (totalFrames - 1) * frameSize.x, 0 }, frameSize));
        }
    }
}

void DeadState::draw(sf::RenderWindow& window, const sf::Sprite& playerSprite, const Exhaust&) const {
    window.draw(playerSprite);
}