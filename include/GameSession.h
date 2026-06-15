#pragma once
#include "Board.h"
#include <SFML/Graphics.hpp>

class GameSession {
public:
    GameSession();

    void update(float deltaTime, bool isThrusting);
    void drawWorld(sf::RenderWindow& window) const;

    int getScore() const;
    int getLives() const;
    bool isGameOver() const;
    sf::Vector2f getPlayerPosition() const;
    int getDistanceInMeters() const;

    // Fonctions pour manipuler les données depuis le Board ou CollisionManager plus tard
    void addScore(int points);
    void removeLife();

private:
    Board m_board;
    int m_score;
    int m_lives;
    float m_gameSpeed;
};