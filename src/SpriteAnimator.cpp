#include "SpriteAnimator.h"

SpriteAnimator::SpriteAnimator(const sf::Texture& texture, int frameCount, float frameDuration)
    : m_frameDuration(frameDuration)
{
    float frameW = static_cast<float>(texture.getSize().x) / frameCount;
    float frameH = static_cast<float>(texture.getSize().y);

    for (int i = 0; i < frameCount; ++i) {
        m_frames.push_back(sf::IntRect(
            { static_cast<int>(i * frameW), 0 },
            { static_cast<int>(frameW), static_cast<int>(frameH) }
        ));
    }
}

bool SpriteAnimator::update(float deltaTime) {
    if (m_frames.size() <= 1) return false;

    m_frameTimer += deltaTime;
    if (m_frameTimer >= m_frameDuration) {
        m_frameTimer = 0.f;
        m_currentFrame = (m_currentFrame + 1) % m_frames.size();
        return true;
    }
    return false;
}

void SpriteAnimator::applyTo(sf::Sprite& sprite) const {
    if (!m_frames.empty())
        sprite.setTextureRect(m_frames[m_currentFrame]);
}

void SpriteAnimator::reset() {
    m_currentFrame = 0;
    m_frameTimer = 0.f;
}

sf::Vector2i SpriteAnimator::getFrameSize() const {
    if (m_frames.empty()) return { 0, 0 };
    return { m_frames[0].size.x, m_frames[0].size.y };
}