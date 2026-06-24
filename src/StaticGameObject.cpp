#include "StaticGameObject.h"

StaticGameObject::StaticGameObject(const sf::Texture& texture,
    const sf::Vector2f& position,
    int frameCount)
    : Object(texture),
    m_animator(texture, frameCount)
{
    m_animator.applyTo(m_sprite);

    auto size = m_animator.getFrameSize();
    m_sprite.setOrigin({ size.x / 2.f, size.y / 2.f });
    m_sprite.setPosition(position);
    m_sprite.setScale({ 1.f, 1.f });
}

void StaticGameObject::update(float deltaTime) {
    if (m_animator.update(deltaTime))
        m_animator.applyTo(m_sprite);
}