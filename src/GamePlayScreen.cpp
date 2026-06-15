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
    // CORRECTION : Au lieu de figer l'écran, on coupe juste les commandes de saut si Game Over
    bool isThrusting = m_session.isGameOver() ? false : m_inputHandler.isThrustingActive();

    // 1. On laisse la mise à jour s'exécuter (permet au DeadState du joueur de tourner !)
    m_session.update(deltaTime, isThrusting);

    // 2. Synchronisation de l'affichage de l'interface
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