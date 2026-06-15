#pragma once
#include <vector>
#include <memory>
#include "Object.h"

class Player;

class LevelGenerator {
public:
    LevelGenerator();
    ~LevelGenerator() = default;

    void generate(float deltaTime,
        std::vector<std::unique_ptr<Object>>& objects,
        Player& player);

private:
    // --- Sous-responsabilites privees ---
    void  spawnMissiles(float deltaTime,
        std::vector<std::unique_ptr<Object>>& objects,
        Player& player, float playerX);

    float spawnLaser(std::vector<std::unique_ptr<Object>>& objects,
        float spawnX);

    // --- Etat ---
    float m_nextSpawnX;
    float m_missileTimer;
    float m_nextMissileDelay;

    // --- Constantes ---
    static constexpr float COIN_SPACING = 48.0f;
    static constexpr float CEILING_LIMIT = 50.0f;
    static constexpr float FLOOR_LIMIT = 530.0f;
    static constexpr float DISTANCE_SECURITE_DEBUT = 15000.0f;
    static constexpr float LASER_RADIUS = 175.0f;
};