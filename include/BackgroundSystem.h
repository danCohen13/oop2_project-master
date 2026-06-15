#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class BackgroundSystem {
public:
    BackgroundSystem(float screenWidth, float screenHeight);
    ~BackgroundSystem() = default;

    void update(float deltaTime, float playerX);
    void draw(sf::RenderWindow& window) const;

private:
    sf::Sprite m_startBackground;
    std::vector<sf::Sprite> m_backgrounds;

    float m_screenWidth;
    float m_screenHeight;
};