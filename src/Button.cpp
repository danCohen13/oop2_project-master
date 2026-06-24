#include "Button.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


sf::ConvexShape Button::makeRoundedRect(const sf::Vector2f& size, float radius) {
    const int segmentsPerCorner = 6; 
    std::vector<sf::Vector2f> points;

    struct CornerCenter { float cx, cy, startAngleDeg; };
    CornerCenter corners[4] = {
        { radius,            radius,            180.f }, 
        { size.x - radius,   radius,            270.f }, 
        { size.x - radius,   size.y - radius,     0.f }, 
        { radius,            size.y - radius,     90.f }, 
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


Button::Button(const sf::Vector2f& position, const sf::Vector2f& size,
    const std::string& textStr, const sf::Font& font)
    : m_text(font),
    m_normalColor(sf::Color(40, 60, 90, 110)),
    m_hoverColor(sf::Color(70, 110, 160, 150)),
    m_borderColorNormal(sf::Color(180, 220, 255, 90)),
    m_borderColorHover(sf::Color(120, 220, 255, 220)),
    m_highlightColor(sf::Color(255, 255, 255, 35)),
    m_size(size),
    m_cornerRadius(std::min(size.x, size.y) * 0.22f), 
    m_currentScale(1.0f),
    m_targetScale(1.0f),
    m_currentGlow(0.0f),
    m_targetGlow(0.0f)
{
    m_glassBase = makeRoundedRect(size, m_cornerRadius);
    m_glassBase.setFillColor(m_normalColor);
    m_glassBase.setOrigin({ size.x / 2.0f, size.y / 2.0f });
    m_glassBase.setPosition(position);

    m_borderOutline = m_glassBase;
    m_borderOutline.setFillColor(sf::Color::Transparent);
    m_borderOutline.setOutlineColor(m_borderColorNormal);
    m_borderOutline.setOutlineThickness(1.5f);

    float highlightHeight = size.y * 0.35f;
    m_glassHighlight = makeRoundedRect({ size.x, highlightHeight }, m_cornerRadius);
    m_glassHighlight.setFillColor(m_highlightColor);
    m_glassHighlight.setOrigin({ size.x / 2.0f, size.y / 2.0f });
    m_glassHighlight.setPosition(position);

    m_text.setString(textStr);
    m_text.setCharacterSize(24);
    m_text.setFillColor(sf::Color::White);
    m_text.setStyle(sf::Text::Bold);

    auto textSize = m_text.getLocalBounds().size;
    m_text.setOrigin({ textSize.x / 2.0f, textSize.y / 2.0f });
    m_text.setPosition(position);
}


void Button::update(const sf::Vector2f& mousePos, float deltaTime) {
    bool isHovered = m_glassBase.getGlobalBounds().contains(mousePos);

    if (isHovered) {
        m_targetScale = 1.06f; 
        m_targetGlow = 1.0f;
    }
    else {
        m_targetScale = 1.0f;
        m_targetGlow = 0.0f;
    }

    m_currentScale += (m_targetScale - m_currentScale) * 10.0f * deltaTime;
    m_currentGlow += (m_targetGlow - m_currentGlow) * 10.0f * deltaTime;

    m_glassBase.setScale({ m_currentScale, m_currentScale });
    m_glassHighlight.setScale({ m_currentScale, m_currentScale });
    m_borderOutline.setScale({ m_currentScale, m_currentScale });
    m_text.setScale({ m_currentScale, m_currentScale });

    auto lerpColor = [](const sf::Color& a, const sf::Color& b, float t) {
        return sf::Color(
            static_cast<std::uint8_t>(a.r + (b.r - a.r) * t),
            static_cast<std::uint8_t>(a.g + (b.g - a.g) * t),
            static_cast<std::uint8_t>(a.b + (b.b - a.b) * t),
            static_cast<std::uint8_t>(a.a + (b.a - a.a) * t)
        );
        };

    m_glassBase.setFillColor(lerpColor(m_normalColor, m_hoverColor, m_currentGlow));
    m_borderOutline.setOutlineColor(lerpColor(m_borderColorNormal, m_borderColorHover, m_currentGlow));
}


void Button::draw(sf::RenderWindow& window) const {
    window.draw(m_glassBase);      
    window.draw(m_glassHighlight); 
    window.draw(m_borderOutline);  
    window.draw(m_text);           
}

bool Button::isClicked(const sf::Vector2f& mousePos) const {
    return m_glassBase.getGlobalBounds().contains(mousePos);
}

void Button::setPosition(const sf::Vector2f& position) {
    m_glassBase.setPosition(position);
    m_glassHighlight.setPosition(position);
    m_borderOutline.setPosition(position);
    m_text.setPosition(position);
}

sf::FloatRect Button::getGlobalBounds() const {
    return m_glassBase.getGlobalBounds();
}