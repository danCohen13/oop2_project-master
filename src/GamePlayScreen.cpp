#include "GameplayScreen.h"
#include "Resources.h"
#include <cmath>

GameplayScreen::GameplayScreen(ScreenStack& stack)
    : Screen(stack),
    m_session(std::make_unique<GameSession>()), // CORRECTION : Instanciation dynamique
    m_hud(),
    m_backgroundSystem(SCREEN_WIDTH, SCREEN_HEIGHT),
    m_pauseHUDButton(Resources::getInstance().getTexture("pause_btn")),
    m_isPaused(false),
    m_mousePos(0.f, 0.f)
{
    // 1. Positionnement du petit bouton de pause HUD (en haut à droite)
    auto pauseSize = m_pauseHUDButton.getLocalBounds().size;
    m_pauseHUDButton.setOrigin({ pauseSize.x / 2.0f, pauseSize.y / 2.0f });
    m_pauseHUDButton.setPosition({ 1160.0f, 35.0f });

    // 2. Configuration du panneau transparent d'ombrage
    m_pauseOverlay.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
    m_pauseOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    // 3. Initialisation des boutons de pause au centre de la vue
    const auto& font = Resources::getInstance().getFont();
    sf::Vector2f btnSize{ 260.0f, 60.0f };

    m_resumeButton = std::make_unique<Button>(sf::Vector2f{ 600.f, 220.f }, btnSize, "Resume", font);
    m_restartButton = std::make_unique<Button>(sf::Vector2f{ 600.f, 320.f }, btnSize, "Restart", font);
    m_menuButton = std::make_unique<Button>(sf::Vector2f{ 600.f, 420.f }, btnSize, "Menu", font);
}

void GameplayScreen::handleEvent(const sf::Event& event) {
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        m_mousePos = { static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y) };
    }

    if (m_inputHandler.isEscapePressed(event)) {
        m_isPaused = !m_isPaused;
        return;
    }

    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            sf::Vector2f clickPos{ static_cast<float>(mouseReleased->position.x), static_cast<float>(mouseReleased->position.y) };

            if (m_isPaused) {
                if (m_resumeButton->isClicked(clickPos)) {
                    m_isPaused = false;
                }
                else if (m_restartButton->isClicked(clickPos)) {
                    // CORRECTION : Recréation propre et autorisée d'une nouvelle session de jeu
                    m_session = std::make_unique<GameSession>();
                    m_isPaused = false;
                }
                else if (m_menuButton->isClicked(clickPos)) {
                    m_stack.pop();
                }
            }
            else {
                if (m_pauseHUDButton.getGlobalBounds().contains(clickPos)) {
                    m_isPaused = true;
                }
            }
        }
    }
}

void GameplayScreen::update(float deltaTime) {
    if (m_isPaused) {
        m_resumeButton->update(m_mousePos, deltaTime);
        m_restartButton->update(m_mousePos, deltaTime);
        m_menuButton->update(m_mousePos, deltaTime);
        return;
    }

    // Accès aux membres par pointeur (->)
    bool isThrusting = m_session->isGameOver() ? false : m_inputHandler.isThrustingActive();

    m_session->update(deltaTime, isThrusting);
    m_hud.updateTexts(m_session->getScore(), m_session->getLives(), m_session->getDistanceInMeters());
    m_backgroundSystem.update(deltaTime, m_session->getPlayerPosition().x);
}

void GameplayScreen::draw(sf::RenderWindow& window) {
    // --- PARTIE 1 : DESSIN DU MONDE (AVEC CAMERA MOBILE) ---
    sf::View camera = window.getDefaultView();
    float cameraX = m_session->getPlayerPosition().x + (SCREEN_WIDTH / 3.0f);

    if (cameraX < SCREEN_WIDTH / 2.0f) {
        cameraX = SCREEN_WIDTH / 2.0f;
    }

    camera.setCenter({ std::round(cameraX), std::round(SCREEN_HEIGHT / 2.0f) });
    window.setView(camera);

    m_backgroundSystem.draw(window);
    m_session->drawWorld(window);

    // --- PARTIE 2 : DESSIN DE L'INTERFACE UTILISATEUR (CAMERA FIXE) ---
    window.setView(window.getDefaultView());
    m_hud.draw(window);

    if (!m_isPaused) {
        window.draw(m_pauseHUDButton);
    }

    // --- PARTIE 3 : PANNEAU DE PAUSE ---
    if (m_isPaused) {
        window.draw(m_pauseOverlay);
        m_resumeButton->draw(window);
        m_restartButton->draw(window);
        m_menuButton->draw(window);
    }
}