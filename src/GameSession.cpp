#include "GameSession.h"

GameSession::GameSession()
    : m_score(0), m_lives(1), m_gameSpeed(140.0f)
{
}

void GameSession::update(float deltaTime, bool isThrusting) {
    // La vitesse augmente en permanence
    if (m_gameSpeed < 900.0f) {
        m_gameSpeed += 3.5f * deltaTime;
    }

    // On donne le top départ à la physique
    m_board.play(deltaTime, m_gameSpeed, isThrusting);

    // On récupère les fruits du travail du Board
    int coins = m_board.getCoinsCollectedThisFrame();
    if (coins > 0) {
        addScore(1); // +10 points par pièce ramassée !
    }

    if (m_board.hasPlayerHitHazard()) {
        removeLife();
    }
}

void GameSession::drawWorld(sf::RenderWindow& window) const {
    m_board.draw(window); // Délègue le dessin au moteur physique
}

int GameSession::getScore() const { return m_score; }
int GameSession::getLives() const { return m_lives; }
bool GameSession::isGameOver() const { return m_lives <= 0 || !m_board.isPlayerAlive(); }
sf::Vector2f GameSession::getPlayerPosition() const { return m_board.getPlayerPosition(); }

int GameSession::getDistanceInMeters() const {
    float playerX = getPlayerPosition().x;

    // Le joueur commence à X = 100, on soustrait 100 pour démarrer à 0m.
    float distanceInPixels = playerX - 100.0f;

    // Règle de conversion : 100 pixels = 1 mètre
    int meters = static_cast<int>(distanceInPixels / 100.0f);

    return (meters < 0) ? 0 : meters; // Sécurité pour éviter les nombres négatifs au tout début
}

void GameSession::addScore(int points) { m_score += points; }
void GameSession::removeLife() { m_lives--; }