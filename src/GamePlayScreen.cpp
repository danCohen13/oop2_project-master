#include "GameplayScreen.h"
#include <cmath>

GameplayScreen::GameplayScreen(ScreenStack& stack)
    : Screen(stack),
    m_session(),
    m_hud(),
    m_backgroundSystem(SCREEN_WIDTH, SCREEN_HEIGHT)
{
}

void GameplayScreen::handleEvent(const sf::Event& event) {
    if (m_inputHandler.isEscapePressed(event)) {
        m_stack.pop();
    }
}

void GameplayScreen::update(float deltaTime) {
    // Si la partie est perdue, on gèle l'update
    if (m_session.isGameOver()) return;

    bool isThrusting = m_inputHandler.isThrustingActive();

    // 1. Mise à jour des règles métier et de la physique
    m_session.update(deltaTime, isThrusting);

    // 2. Synchronisation de l'affichage avec les données actuelles
    m_hud.updateTexts(m_session.getScore(), m_session.getLives(), m_session.getDistanceInMeters());

    // 3. Défilement de l'arrière-plan
    m_backgroundSystem.update(deltaTime, m_session.getPlayerPosition().x);
}

void GameplayScreen::draw(sf::RenderWindow& window) {
    // --- PARTIE 1 : DESSIN DU MONDE (AVEC CAMERA MOBILE) ---
    sf::View camera = window.getDefaultView();
    float cameraX = m_session.getPlayerPosition().x + (SCREEN_WIDTH / 3.0f);

    if (cameraX < SCREEN_WIDTH / 2.0f) {
        cameraX = SCREEN_WIDTH / 2.0f;
    }

    camera.setCenter({ std::round(cameraX), std::round(SCREEN_HEIGHT / 2.0f) });
    window.setView(camera);

    m_backgroundSystem.draw(window);
    m_session.drawWorld(window);

    // --- PARTIE 2 : DESSIN DE L'INTERFACE (AVEC CAMERA FIXE) ---
    // C'est vital de remettre la vue par défaut pour que le texte 
    // ne parte pas hors de l'écran avec les déplacements du joueur !
    window.setView(window.getDefaultView());
    m_hud.draw(window);
}