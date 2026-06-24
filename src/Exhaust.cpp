#include "Exhaust.h"
#include "Resources.h"

Exhaust::Exhaust()
    : m_sprite(Resources::getInstance().getTexture("Exhaust")),
    m_animator(Resources::getInstance().getTexture("Exhaust"), 6) 
{
    m_animator.applyTo(m_sprite);

    auto size = m_animator.getFrameSize();
    m_sprite.setOrigin({ static_cast<float>(size.x) / 2.f, 0.f });
}

void Exhaust::setActive(bool active) {
    if (!active && m_active) {
        m_animator.reset();
        m_animator.applyTo(m_sprite);
    }
    m_active = active;
}

void Exhaust::update(float deltaTime, const sf::Vector2f& playerPos) {
    if (!m_active) return;

    if (m_animator.update(deltaTime))
        m_animator.applyTo(m_sprite);

    m_sprite.setPosition({ playerPos.x + OFFSET_X, playerPos.y + OFFSET_Y });
}

void Exhaust::syncFrame(int playerFrame) {
    if (!m_active) return;

    const sf::Texture& tex = Resources::getInstance().getTexture("Exhaust");
    int fw = tex.getSize().x / 6;
    int fh = tex.getSize().y;
    m_sprite.setTextureRect(sf::IntRect({ playerFrame * fw, 0 }, { fw, fh }));
}

void Exhaust::draw(sf::RenderWindow& window) const {
    if (m_active)
        window.draw(m_sprite);
}