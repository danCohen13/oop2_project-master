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

    void play(float deltaTime, float gameSpeed, bool isThrusting);
    void draw(sf::RenderWindow& window) const;

    bool isPlayerAlive() const;
    sf::Vector2f getPlayerPosition() const;
    Player* getPlayer() const { return m_player.get(); }

    int getCoinsCollectedThisFrame() const;
    bool hasPlayerHitHazard() const;

private:
    std::unique_ptr<Player> m_player;
    std::vector<std::unique_ptr<Object>> m_objects;

    std::unique_ptr<CollisionManager> m_collisionManager;
    std::unique_ptr<LevelGenerator> m_levelGenerator;
    std::unique_ptr<ObjectCleaner> m_objectCleaner;

    int m_coinsCollected;
    bool m_hazardHit;
};