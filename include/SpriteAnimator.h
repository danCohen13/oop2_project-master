#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class SpriteAnimator {
public:
    SpriteAnimator(const sf::Texture& texture, int frameCount, float frameDuration = 0.1f);
    SpriteAnimator(const sf::Texture& texture, std::vector<sf::IntRect> frames, float frameDuration = 0.1f);

    bool update(float deltaTime);

    void applyTo(sf::Sprite& sprite) const;

    void reset();

    sf::Vector2i getFrameSize() const;

    int getCurrentFrame() const { return static_cast<int>(m_currentFrame); }

private:
    std::vector<sf::IntRect> m_frames;
    size_t m_currentFrame = 0;
    float  m_frameTimer = 0.f;
    float  m_frameDuration;
};