#include "LevelGenerator.h"
#include "CoinFormation.h"
#include "GameObjectFactory.h"
#include "Missile.h"
#include "Player.h"
#include <algorithm>
#include <cstdlib>

LevelGenerator::LevelGenerator()
    : m_nextSpawnX(0.0f),
    m_missileTimer(0.0f),
    m_nextMissileDelay(4.0f + static_cast<float>(rand() % 4))
{
}

void LevelGenerator::generate(float deltaTime,
    std::vector<std::unique_ptr<Object>>& objects,
    Player& player)
{
    float playerX = player.getPosition().x;
    float currentSpawnZoneX = playerX + 1400.0f;

    // 1. Missiles — flux temporel independant
    spawnMissiles(deltaTime, objects, player, playerX);

    // 2. Coins / Lasers — flux spatial base sur m_nextSpawnX
    if (m_nextSpawnX == 0.0f)
        m_nextSpawnX = currentSpawnZoneX;

    if (currentSpawnZoneX >= m_nextSpawnX) {
        float spawnX = m_nextSpawnX;
        float generationWidth = 0.0f;

        if (rand() % 10 < 6)
            // 60% — formation de pieces (switch delegue a CoinFormation)
            generationWidth = CoinFormation::createRandom(
                objects, spawnX, CEILING_LIMIT, FLOOR_LIMIT, COIN_SPACING);
        else
            // 40% — laser
            generationWidth = spawnLaser(objects, spawnX);

        float separationPadding = 500.0f + static_cast<float>(rand() % 600);
        m_nextSpawnX = spawnX + generationWidth + separationPadding;
    }
}

void LevelGenerator::spawnMissiles(float deltaTime,
    std::vector<std::unique_ptr<Object>>& objects,
    Player& player, float playerX)
{
    // Zone de securite au debut : pas de missiles avant 15 000px
    if (player.isDead() || playerX <= DISTANCE_SECURITE_DEBUT) return;

    m_missileTimer += deltaTime;
    if (m_missileTimer < m_nextMissileDelay) return;

    // Reinitialisation du timer
    m_missileTimer = 0.0f;

    // Difficulte croissante : delai minimum diminue avec la distance
    float delaiMin = std::max(2.5f, 7.0f - (playerX / 20000.0f));
    m_nextMissileDelay = delaiMin + static_cast<float>(rand() % 4);

    // Taille de la salve : +1 missile tous les 10 000px, plafond a 3
    int salve = std::min(
        1 + static_cast<int>((playerX - DISTANCE_SECURITE_DEBUT) / 10000.0f),
        3
    );

    if (salve == 1) {
        objects.push_back(std::make_unique<Missile>(player, 0.0f));
    }
    else if (salve == 2) {
        objects.push_back(std::make_unique<Missile>(player, -130.0f));
        objects.push_back(std::make_unique<Missile>(player, 130.0f));
    }
    else {
        objects.push_back(std::make_unique<Missile>(player, 0.0f));
        objects.push_back(std::make_unique<Missile>(player, -190.0f));
        objects.push_back(std::make_unique<Missile>(player, 190.0f));
    }
}

float LevelGenerator::spawnLaser(std::vector<std::unique_ptr<Object>>& objects,
    float spawnX)
{
    // Laser place aleatoirement en haut ou en bas de l'ecran
    float laserY = (rand() % 2 == 0)
        ? CEILING_LIMIT + LASER_RADIUS
        : FLOOR_LIMIT - LASER_RADIUS;

    objects.push_back(
        GameObjectFactory::createObject("Laser", { spawnX + 200.0f, laserY })
    );
    return 400.0f;
}