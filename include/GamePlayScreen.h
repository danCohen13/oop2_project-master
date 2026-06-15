#pragma once
#include "Screen.h"
#include "GameSession.h"
#include "HUD.h"
#include "BackgroundSystem.h"
#include "InputHandler.h" 

class GameplayScreen : public Screen {
public:
    GameplayScreen(ScreenStack& stack);

    virtual void handleEvent(const sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void draw(sf::RenderWindow& window) override;

private:
    static constexpr float SCREEN_WIDTH = 1200.0f;
    static constexpr float SCREEN_HEIGHT = 600.0f;

    GameSession m_session;     // Remplace le Board
    HUD m_hud;                 // Nouvel affichage IHM
    BackgroundSystem m_backgroundSystem;
    InputHandler m_inputHandler;
};