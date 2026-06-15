#pragma once
#include <vector>
#include <memory>
#include "Object.h"

class Player;

class LevelGenerator {
public:
    LevelGenerator();
    ~LevelGenerator() = default;

    void generate(float deltaTime, std::vector<std::unique_ptr<Object>>& objects, Player& player);

private:
    float m_nextSpawnX;

    // NOUVEAU : Infrastructure de chronométrage indépendante pour les missiles
    float m_missileTimer;
    float m_nextMissileDelay;

    const float COIN_SPACING = 40.0f;
    const float CEILING_LIMIT = 50.0f;
    const float FLOOR_LIMIT = 530.0f;
};