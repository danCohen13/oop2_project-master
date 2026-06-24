#pragma once
#include <vector>
#include <memory>
#include "Object.h"

class CoinFormation {
public:
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

    static float createRandom(std::vector<std::unique_ptr<Object>>& objects,
        float spawnX,
        float ceilingLimit,
        float floorLimit,
        float coinSpacing);

private:
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