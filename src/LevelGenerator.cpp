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

void LevelGenerator::generate(float deltaTime,std::vector<std::unique_ptr<Object>>& objects,Player& player)
{
    float playerX = player.getPosition().x;
    float currentSpawnZoneX = playerX + 1400.0f;

    // 1. Missiles — flux temporel indépendant
    spawnMissiles(deltaTime, objects, player, playerX);

    // 2. Coins / Lasers / SpeedItems / PowerUpBox — flux spatial basé sur m_nextSpawnX
    if (m_nextSpawnX == 0.0f)
        m_nextSpawnX = currentSpawnZoneX;

    if (currentSpawnZoneX >= m_nextSpawnX) {
        float spawnX = m_nextSpawnX;
        float generationWidth = 0.0f;

        int roll = rand() % 100;
        if (roll < 60) { // 0 à 59 : Pièces (60% de chance)
            generationWidth = CoinFormation::createRandom(
                objects, spawnX, CEILING_LIMIT, FLOOR_LIMIT, COIN_SPACING);
        }
        else if (roll < 85) { // 60 à 84 : Lasers (25% de chance)
            generationWidth = spawnLaser(objects, spawnX);
        }
        else if (roll < 95) { // 85 à 94 : SpeedItem (10% de chance)
            float itemY = CEILING_LIMIT + 100.0f + static_cast<float>(rand() % static_cast<int>(FLOOR_LIMIT - CEILING_LIMIT - 200.0f));
            objects.push_back(GameObjectFactory::createObject("SpeedItem", { spawnX + 200.0f, itemY }));
            generationWidth = 300.0f;
        }
        else { // 95 à 99 : PowerUpBox (5% de chance) <-- AJOUT FEATURE
            float boxY = CEILING_LIMIT + 100.0f + static_cast<float>(rand() % static_cast<int>(FLOOR_LIMIT - CEILING_LIMIT - 200.0f));
            objects.push_back(GameObjectFactory::createObject("PowerUpBox", { spawnX + 200.0f, boxY }));
            generationWidth = 300.0f;
        }

        // --- GÉNÉRATION ALÉATOIRE DE SCIENTIFIQUES AU SOL ---
        if (rand() % 10 < 4) { // 40% de chance d'apparition par section de carte
            float scientistX = spawnX + static_cast<float>(rand() % 500);
            objects.push_back(GameObjectFactory::createObject("Scientist", { scientistX, FLOOR_LIMIT }));
        }

        float separationPadding = 500.0f + static_cast<float>(rand() % 600);
        m_nextSpawnX = spawnX + generationWidth + separationPadding;
    }
}

void LevelGenerator::spawnMissiles(float deltaTime,
    std::vector<std::unique_ptr<Object>>& objects,
    Player& player, float playerX)
{
    if (player.isDead() || playerX <= DISTANCE_SECURITE_DEBUT) return;

    m_missileTimer += deltaTime;
    if (m_missileTimer < m_nextMissileDelay) return;

    m_missileTimer = 0.0f;

    float delaiMin = std::max(2.5f, 7.0f - (playerX / 20000.0f));
    m_nextMissileDelay = delaiMin + static_cast<float>(rand() % 4);

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
    float laserY = (rand() % 2 == 0)
        ? CEILING_LIMIT + LASER_RADIUS
        : FLOOR_LIMIT - LASER_RADIUS;

    objects.push_back(
        GameObjectFactory::createObject("Laser", { spawnX + 200.0f, laserY })
    );
    return 400.0f;
}