#pragma once
#include "Screen.h"
#include "GameSession.h"
#include "HUD.h"
#include "BackgroundSystem.h"
#include "InputHandler.h" 
#include "Button.h"
#include <memory>

class GameplayScreen : public Screen {
public:
    GameplayScreen(ScreenStack& stack);

    virtual void handleEvent(const sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) override;

private:
    static constexpr float SCREEN_WIDTH = 1200.0f;
    static constexpr float SCREEN_HEIGHT = 600.0f;

    std::unique_ptr<GameSession> m_session; // CORRECTION : Utilisation d'un unique_ptr pour le Restart
    HUD m_hud;
    BackgroundSystem m_backgroundSystem;
    InputHandler m_inputHandler;

    // Composants du système de pause
    sf::Sprite m_pauseHUDButton;
    bool m_isPaused;
    sf::RectangleShape m_pauseOverlay;

    std::unique_ptr<Button> m_resumeButton;
    std::unique_ptr<Button> m_restartButton;
    std::unique_ptr<Button> m_menuButton;

    sf::Vector2f m_mousePos;
};