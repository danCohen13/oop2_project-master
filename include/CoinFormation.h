#pragma once
#include <vector>
#include <memory>
#include "Object.h"

class CoinFormation {
public:
    // --- Formations publiques utilisables directement ---
    static void createLine(std::vector<std::unique_ptr<Object>>& objects,
        float startX, float startY,
        float spacing, int length, int thickness);

    static void createDiagonal(std::vector<std::unique_ptr<Object>>& objects,
        float startX, float startY,
        float spacing, int length);

    static void createWave(std::vector<std::unique_ptr<Object>>& objects,
        float startX, float startY,
        float spacing, int length);

    static void createTriangle(std::vector<std::unique_ptr<Object>>& objects,
        float startX, float startY,
        float spacing, int size);

    static void createCircle(std::vector<std::unique_ptr<Object>>& objects,
        float startX, float startY,
        float spacing, int radiusFactor);

    // --- Choix aleatoire d'une formation ---
    // Retourne la largeur generee (generationWidth) pour que LevelGenerator
    // puisse calculer m_nextSpawnX sans connaitre les details de chaque formation.
    static float createRandom(std::vector<std::unique_ptr<Object>>& objects,
        float spawnX,
        float ceilingLimit,
        float floorLimit,
        float coinSpacing);

private:
    // --- Createurs aleatoires prives (un par formation) ---
    // Chacun gere ses propres parametres aleatoires et retourne sa largeur.
    static float createLineRandom(std::vector<std::unique_ptr<Object>>& objects,
        float spawnX, float ceiling,
        float floor, float spacing);

    static float createDiagonalRandom(std::vector<std::unique_ptr<Object>>& objects,
        float spawnX, float ceiling,
        float floor, float spacing);

    static float createWaveRandom(std::vector<std::unique_ptr<Object>>& objects,
        float spawnX, float ceiling,
        float floor, float spacing);

    static float createTriangleRandom(std::vector<std::unique_ptr<Object>>& objects,
        float spawnX, float ceiling,
        float floor, float spacing);

    static float createCircleRandom(std::vector<std::unique_ptr<Object>>& objects,
        float spawnX, float ceiling,
        float floor, float spacing);
};