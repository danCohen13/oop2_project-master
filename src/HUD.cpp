#include "HUD.h"
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

sf::ConvexShape HUD::makeRoundedRect(const sf::Vector2f& size, float radius) {
    const int segmentsPerCorner = 6;
    std::vector<sf::Vector2f> points;

    struct CornerCenter { float cx, cy, startAngleDeg; };
    CornerCenter corners[4] = {
        { radius,          radius,          180.f }, 
        { size.x - radius, radius,          270.f }, 
        { size.x - radius, size.y - radius,   0.f }, 
        { radius,          size.y - radius,  90.f }, 
    };

    for (const auto& c : corners) {
        for (int i = 0; i <= segmentsPerCorner; ++i) {
            float angle = (c.startAngleDeg + (90.f * i / segmentsPerCorner)) * (M_PI / 180.f);
            points.push_back({
                c.cx + radius * std::cos(angle),
                c.cy + radius * std::sin(angle)
                });
        }
    }

    sf::ConvexShape shape;
    shape.setPointCount(points.size());
    for (size_t i = 0; i < points.size(); ++i)
        shape.setPoint(i, points[i]);

    return shape;
}

HUD::HUD()
    : m_font(),
    m_scoreText(m_font, ""),
    m_livesText(m_font, ""),
    m_distanceText(m_font, "")
{
    if (!m_font.openFromFile("resources/New Athletic M54.ttf")) {
        std::cerr << "Error: Impossible to charge the police for the HUD.\n";
    }

    const float cornerRadius = 14.f;

    sf::Vector2f leftPanelSize{ 230.f, 90.f };
    sf::Vector2f leftPanelPos{ 20.f, 15.f }; 

    m_leftPanelBase = makeRoundedRect(leftPanelSize, cornerRadius);
    m_leftPanelBase.setFillColor(sf::Color(40, 60, 95, 100));
    m_leftPanelBase.setPosition(leftPanelPos);

    m_leftPanelBorder = m_leftPanelBase;
    m_leftPanelBorder.setFillColor(sf::Color::Transparent);
    m_leftPanelBorder.setOutlineColor(sf::Color(150, 210, 255, 110));
    m_leftPanelBorder.setOutlineThickness(1.5f);

    float leftHighlightH = leftPanelSize.y * 0.4f;
    m_leftPanelHighlight = makeRoundedRect({ leftPanelSize.x, leftHighlightH }, cornerRadius);
    m_leftPanelHighlight.setFillColor(sf::Color(255, 255, 255, 25));
    m_leftPanelHighlight.setPosition(leftPanelPos);

    m_scoreText.setCharacterSize(26);
    m_scoreText.setFillColor(sf::Color::White);
    m_scoreText.setStyle(sf::Text::Bold);
    m_scoreText.setPosition({ leftPanelPos.x + 18.f, leftPanelPos.y + 8.f });

    m_livesText.setCharacterSize(22);
    m_livesText.setFillColor(sf::Color(255, 110, 110));
    m_livesText.setPosition({ leftPanelPos.x + 18.f, leftPanelPos.y + 48.f });

    sf::Vector2f rightPanelSize{ 150.f, 56.f };
    sf::Vector2f rightPanelPos{ 1200.f - rightPanelSize.x - 20.f, 15.f };

    m_rightPanelBase = makeRoundedRect(rightPanelSize, cornerRadius);
    m_rightPanelBase.setFillColor(sf::Color(40, 60, 95, 100));
    m_rightPanelBase.setPosition(rightPanelPos);

    m_rightPanelBorder = m_rightPanelBase;
    m_rightPanelBorder.setFillColor(sf::Color::Transparent);
    m_rightPanelBorder.setOutlineColor(sf::Color(150, 210, 255, 110));
    m_rightPanelBorder.setOutlineThickness(1.5f);

    float rightHighlightH = rightPanelSize.y * 0.4f;
    m_rightPanelHighlight = makeRoundedRect({ rightPanelSize.x, rightHighlightH }, cornerRadius);
    m_rightPanelHighlight.setFillColor(sf::Color(255, 255, 255, 25));
    m_rightPanelHighlight.setPosition(rightPanelPos);

    m_distanceText.setCharacterSize(26);
    m_distanceText.setFillColor(sf::Color(255, 220, 110));
    m_distanceText.setStyle(sf::Text::Bold);
    m_distanceText.setPosition({ rightPanelPos.x + 18.f, rightPanelPos.y + 14.f });

    updateTexts(0, 1, 0);
}

void HUD::updateTexts(int score, int lives, int distance) {
    m_scoreText.setString("Score: " + std::to_string(score));
    m_livesText.setString("Lives: " + std::to_string(lives));
    m_distanceText.setString(std::to_string(distance) + " m");
}

void HUD::draw(sf::RenderWindow& window) const {
    window.draw(m_leftPanelBase);
    window.draw(m_leftPanelHighlight);
    window.draw(m_leftPanelBorder);
    window.draw(m_scoreText);
    window.draw(m_livesText);

    window.draw(m_rightPanelBase);
    window.draw(m_rightPanelHighlight);
    window.draw(m_rightPanelBorder);
    window.draw(m_distanceText);
}