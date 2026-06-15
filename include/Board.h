#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Player.h"
#include "Object.h"

class CollisionManager;
class LevelGenerator;
class ObjectCleaner;

class Board {
public:
    Board();
    ~Board();

    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    // NOUVEAU : On injecte la gameSpeed calculée par la GameSession
    void play(float deltaTime, float gameSpeed, bool isThrusting);
    void draw(sf::RenderWindow& window) const;

    bool isPlayerAlive() const;
    sf::Vector2f getPlayerPosition() const;

    // Getters d'événements pour la GameSession
    int getCoinsCollectedThisFrame() const;
    bool hasPlayerHitHazard() const;

private:
    std::unique_ptr<Player> m_player;
    std::vector<std::unique_ptr<Object>> m_objects;

    std::unique_ptr<CollisionManager> m_collisionManager;
    std::unique_ptr<LevelGenerator> m_levelGenerator;
    std::unique_ptr<ObjectCleaner> m_objectCleaner;

    // Variables temporaires stockant les événements de la frame
    int m_coinsCollected;
    bool m_hazardHit;
};