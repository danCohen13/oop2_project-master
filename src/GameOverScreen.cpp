#include "GameOverScreen.h"
#include "GameplayScreen.h"
#include "Resources.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// =============================================================================
// COINS ARRONDIS — meme technique geometrique que dans Button.cpp
// (un ConvexShape ou chaque coin est remplace par un arc de cercle)
// =============================================================================
sf::ConvexShape GameOverScreen::makeRoundedRect(const sf::Vector2f& size, float radius) {
    const int segmentsPerCorner = 8; // panneau plus grand => plus de segments pour rester lisse
    std::vector<sf::Vector2f> points;

    struct CornerCenter { float cx, cy, startAngleDeg; };
    CornerCenter corners[4] = {
        { radius,          radius,          180.f }, // haut-gauche
        { size.x - radius, radius,          270.f }, // haut-droit
        { size.x - radius, size.y - radius,   0.f }, // bas-droit
        { radius,          size.y - radius,  90.f }, // bas-gauche
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

// =============================================================================
// CONSTRUCTEUR
// =============================================================================
GameOverScreen::GameOverScreen(ScreenStack& stack, int coins, int distance)
    : Screen(stack),
    m_gameOverText(Resources::getInstance().getFont()),
    m_statsText(Resources::getInstance().getFont()),
    m_mousePos(0.f, 0.f),
    m_coins(coins),
    m_distance(distance)
{
    // 1. Voile sombre derriere tout (assombrit le jeu en arriere-plan)
    m_overlay.setSize({ 1200.f, 600.f });
    m_overlay.setFillColor(sf::Color(10, 10, 18, 195));

    // 2. Panneau central en verre depoli (glassmorphism)
    sf::Vector2f panelSize{ 560.f, 480.f };
    sf::Vector2f panelCenter{ 600.f, 300.f };
    float cornerRadius = 28.f;

    m_panelBase = makeRoundedRect(panelSize, cornerRadius);
    m_panelBase.setFillColor(sf::Color(40, 60, 95, 95)); // bleu nuit translucide
    m_panelBase.setOrigin({ panelSize.x / 2.f, panelSize.y / 2.f });
    m_panelBase.setPosition(panelCenter);

    // Contour fin lumineux du panneau
    m_panelBorder = m_panelBase;
    m_panelBorder.setFillColor(sf::Color::Transparent);
    m_panelBorder.setOutlineColor(sf::Color(150, 210, 255, 130));
    m_panelBorder.setOutlineThickness(1.5f);

    // Reflet en haut du panneau (signature glassmorphism)
    float highlightHeight = panelSize.y * 0.30f;
    m_panelHighlight = makeRoundedRect({ panelSize.x, highlightHeight }, cornerRadius);
    m_panelHighlight.setFillColor(sf::Color(255, 255, 255, 25));
    m_panelHighlight.setOrigin({ panelSize.x / 2.f, panelSize.y / 2.f });
    m_panelHighlight.setPosition(panelCenter);

    // 3. Titre "YOU LOST !"
    m_gameOverText.setString("YOU LOST !");
    m_gameOverText.setCharacterSize(55);
    m_gameOverText.setFillColor(sf::Color(255, 90, 90));
    m_gameOverText.setStyle(sf::Text::Bold);
    auto goSize = m_gameOverText.getLocalBounds().size;
    m_gameOverText.setOrigin({ goSize.x / 2.f, goSize.y / 2.f });
    m_gameOverText.setPosition({ 600.f, 130.f });

    // 4. Statistiques
    std::string statsStr = "Coins Collected : " + std::to_string(m_coins) + "\n" +
        "Distance Traveled : " + std::to_string(m_distance) + " m\n\n" +
        "Best Score : " + std::to_string(m_coins) + " (Placeholder)";
    m_statsText.setString(statsStr);
    m_statsText.setCharacterSize(24);
    m_statsText.setFillColor(sf::Color(225, 235, 255));
    auto statsSize = m_statsText.getLocalBounds().size;
    m_statsText.setOrigin({ statsSize.x / 2.f, statsSize.y / 2.f });
    m_statsText.setPosition({ 600.f, 270.f });

    // 5. Boutons en glassmorphism (deja geres par la classe Button)
    const auto& font = Resources::getInstance().getFont();
    sf::Vector2f btnSize{ 260.0f, 60.0f };
    m_restartButton = std::make_unique<Button>(sf::Vector2f{ 600.f, 410.f }, btnSize, "Restart", font);
    m_menuButton = std::make_unique<Button>(sf::Vector2f{ 600.f, 495.f }, btnSize, "Menu", font);
}

// =============================================================================
// EVENEMENTS — logique de navigation inchangee (deja securisee)
// =============================================================================
void GameOverScreen::handleEvent(const sf::Event& event) {
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_mousePos = { static_cast<float>(mouseMoved->position.x),
                       static_cast<float>(mouseMoved->position.y) };
    }

    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            sf::Vector2f clickPos{ static_cast<float>(mouseReleased->position.x),
                                   static_cast<float>(mouseReleased->position.y) };

            if (m_restartButton->isClicked(clickPos)) {
                auto& safeStack = m_stack;
                safeStack.pop();
                safeStack.pop();
                safeStack.push(std::make_unique<GameplayScreen>(safeStack));
                return;
            }
            else if (m_menuButton->isClicked(clickPos)) {
                auto& safeStack = m_stack;
                safeStack.pop();
                safeStack.pop();
                return;
            }
        }
    }
}

void GameOverScreen::update(float deltaTime) {
    m_restartButton->update(m_mousePos, deltaTime);
    m_menuButton->update(m_mousePos, deltaTime);
}

// =============================================================================
// DRAW — ordre : voile, puis panneau (base + reflet + bordure), puis textes, puis boutons
// =============================================================================
void GameOverScreen::draw(sf::RenderWindow& window) {
    window.draw(m_overlay);

    window.draw(m_panelBase);
    window.draw(m_panelHighlight);
    window.draw(m_panelBorder);

    window.draw(m_gameOverText);
    window.draw(m_statsText);

    m_restartButton->draw(window);
    m_menuButton->draw(window);
}