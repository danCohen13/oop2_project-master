#include "LevelGenerator.h"
#include "CoinFormation.h"
#include "GameObjectFactory.h"
#include "Missile.h"
#include "Player.h"
#include <cstdlib>

LevelGenerator::LevelGenerator()
    : m_nextSpawnX(0.0f),
    m_missileTimer(0.0f),
    m_nextMissileDelay(4.0f + static_cast<float>(rand() % 4)) // Premier alerte après 4 à 8 secondes de jeu
{
}

void LevelGenerator::generate(float deltaTime, std::vector<std::unique_ptr<Object>>& objects, Player& player) {
    float playerX = player.getPosition().x;
    float currentSpawnZoneX = playerX + 1400.0f;

    // =====================================================================
    // 1. FLUX DES MISSILES (Totalement autonome et temporel)
    // =====================================================================
    if (!player.isDead()) { // On arrête d'envoyer des missiles si le joueur est mort
        m_missileTimer += deltaTime;

        if (m_missileTimer >= m_nextMissileDelay) {
            m_missileTimer = 0.0f;
            // Calcule le délai du prochain missile (ex: toutes les 6 à 11 secondes)
            m_nextMissileDelay = 6.0f + static_cast<float>(rand() % 6);

            // Injection directe dans le monde sans impacter la structure m_nextSpawnX
            objects.push_back(std::make_unique<Missile>(player));
        }
    }

    // =====================================================================
    // 2. FLUX DES OBSTACLES FIXES / PIÈCES (Basé sur la distance m_nextSpawnX)
    // =====================================================================
    if (m_nextSpawnX == 0.0f) {
        m_nextSpawnX = currentSpawnZoneX;
    }

    if (currentSpawnZoneX >= m_nextSpawnX) {
        float spawnX = m_nextSpawnX;
        float generationWidth = 0.0f;

        // Retour au ratio original exclusif (60% Pièces / 40% Laser)
        int eventChoice = rand() % 10;

        if (eventChoice < 6) {
            // -----------------------------------------------------------------
            // ÉVÉNEMENT A : GÉNÉRATION DES PIÈCES
            // -----------------------------------------------------------------
            int formationType = rand() % 5;
            float spawnY = 0.0f;
            const float HALF = COIN_SPACING / 2.0f;

            switch (formationType) {
            case 0: { // LIGNE
                int length = 8 + rand() % 6;
                int thickness = 1 + rand() % 3;
                float minY = CEILING_LIMIT + HALF;
                float maxY = FLOOR_LIMIT - ((thickness - 1) * COIN_SPACING) - HALF;
                if (maxY < minY) maxY = minY;
                spawnY = minY + static_cast<float>(rand() % static_cast<int>(maxY - minY + 1));
                CoinFormation::createLine(objects, spawnX, spawnY, COIN_SPACING, length, thickness);
                generationWidth = length * COIN_SPACING;
                break;
            }
            case 1: { // DIAGONALE
                int length = 4 + rand() % 3;
                float minY = CEILING_LIMIT + HALF;
                float maxY = FLOOR_LIMIT - ((length - 1) * COIN_SPACING) - HALF;
                if (maxY < minY) maxY = minY;
                spawnY = minY + static_cast<float>(rand() % static_cast<int>(maxY - minY + 1));
                CoinFormation::createDiagonal(objects, spawnX, spawnY, COIN_SPACING, length);
                generationWidth = length * COIN_SPACING;
                break;
            }
            case 2: { // VAGUE (SERPENT)
                int length = 12 + rand() % 6;
                float amplitude = COIN_SPACING * 1.4f;
                float minY = CEILING_LIMIT + amplitude + HALF;
                float maxY = FLOOR_LIMIT - amplitude - HALF;
                if (maxY < minY) maxY = minY;
                spawnY = minY + static_cast<float>(rand() % static_cast<int>(maxY - minY + 1));
                CoinFormation::createWave(objects, spawnX, spawnY, COIN_SPACING, length);
                generationWidth = length * (COIN_SPACING * 1.7f);
                break;
            }
            case 3: { // TRIANGLE
                int size = 3 + rand() % 3;
                float minY = CEILING_LIMIT + HALF;
                float maxY = FLOOR_LIMIT - ((size - 1) * COIN_SPACING) - HALF;
                if (maxY < minY) maxY = minY;
                spawnY = minY + static_cast<float>(rand() % static_cast<int>(maxY - minY + 1));
                CoinFormation::createTriangle(objects, spawnX, spawnY, COIN_SPACING, size);
                generationWidth = size * COIN_SPACING;
                break;
            }
            case 4: { // CERCLE
                int radiusFactor = 2;
                float radius = COIN_SPACING * 3.0f;
                float minY = CEILING_LIMIT + radius + HALF;
                float maxY = FLOOR_LIMIT - radius - HALF;
                if (maxY < minY) maxY = minY;
                spawnY = minY + static_cast<float>(rand() % static_cast<int>(maxY - minY + 1));
                CoinFormation::createCircle(objects, spawnX, spawnY, COIN_SPACING, radiusFactor);
                generationWidth = radius * 2.0f;
                break;
            }
            }
        }
        else {
            // -----------------------------------------------------------------
            // ÉVÉNEMENT B : GÉNÉRATION D'UN LASER ANCRÉ
            // -----------------------------------------------------------------
            float laserX = spawnX + 200.0f;
            const float LASER_RADIUS = 175.0f;
            float laserY = 0.0f;

            if (rand() % 2 == 0) {
                laserY = CEILING_LIMIT + LASER_RADIUS;
            }
            else {
                laserY = FLOOR_LIMIT - LASER_RADIUS;
            }

            objects.push_back(GameObjectFactory::createObject("Laser", { laserX, laserY }));
            generationWidth = 400.0f;
        }

        // Espacement standard entre les blocs de décors fixes
        float separationPadding = 500.0f + static_cast<float>(rand() % 600);
        m_nextSpawnX = spawnX + generationWidth + separationPadding;
    }
}