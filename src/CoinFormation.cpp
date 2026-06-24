#include "CoinFormation.h"
#include "GameObjectFactory.h"
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


void CoinFormation::createLine(std::vector<std::unique_ptr<Object>>& objects,
    float startX, float startY,
    float spacing, int length, int thickness)
{
    for (int r = 0; r < thickness; ++r)
        for (int c = 0; c < length; ++c)
            objects.push_back(GameObjectFactory::createObject("Coin",
                { startX + c * spacing, startY + r * spacing }));
}

void CoinFormation::createDiagonal(std::vector<std::unique_ptr<Object>>& objects,
    float startX, float startY,
    float spacing, int length)
{
    for (int i = 0; i < length; ++i)
        objects.push_back(GameObjectFactory::createObject("Coin",
            { startX + i * spacing, startY + i * spacing }));
}

void CoinFormation::createWave(std::vector<std::unique_ptr<Object>>& objects,
    float startX, float startY,
    float spacing, int length)
{
    float frequency = 0.35f;
    float amplitude = spacing * 1.4f;
    float horizontalSpacing = spacing * 1.3f;

    for (int i = 0; i < length; ++i)
        objects.push_back(GameObjectFactory::createObject("Coin", {
            startX + i * horizontalSpacing,
            startY + amplitude * std::sin(i * frequency)
            }));
}

void CoinFormation::createTriangle(std::vector<std::unique_ptr<Object>>& objects,
    float startX, float startY,
    float spacing, int size)
{
    for (int r = 0; r < size; ++r) {
        int   coinsInRow = r + 1;
        float rowWidth = (coinsInRow - 1) * spacing;
        float levelStartX = startX - rowWidth / 2.0f;
        float y = startY + r * spacing;

        for (int c = 0; c < coinsInRow; ++c)
            objects.push_back(GameObjectFactory::createObject("Coin",
                { levelStartX + c * spacing, y }));
    }
}

void CoinFormation::createCircle(std::vector<std::unique_ptr<Object>>& objects,
    float startX, float startY,
    float spacing, int radiusFactor)
{
    float radius = spacing * (static_cast<float>(radiusFactor) + 0.4f);
    int   steps = 8;

    for (int i = 0; i < steps; ++i) {
        float radians = (i * 2.0f * static_cast<float>(M_PI)) / steps;
        objects.push_back(GameObjectFactory::createObject("Coin", {
            startX + radius * std::cos(radians),
            startY + radius * std::sin(radians)
            }));
    }
}


float CoinFormation::createLineRandom(std::vector<std::unique_ptr<Object>>& objects,
    float spawnX, float ceiling,
    float floor, float spacing)
{
    int   length = 8 + rand() % 6;
    int   thickness = 1 + rand() % 3;
    float half = spacing / 2.0f;
    float minY = ceiling + half;
    float maxY = floor - (thickness - 1) * spacing - half;
    if (maxY < minY) maxY = minY;

    float spawnY = minY + static_cast<float>(rand() % static_cast<int>(maxY - minY + 1));
    createLine(objects, spawnX, spawnY, spacing, length, thickness);
    return length * spacing;
}

float CoinFormation::createDiagonalRandom(std::vector<std::unique_ptr<Object>>& objects,
    float spawnX, float ceiling,
    float floor, float spacing)
{
    int   length = 4 + rand() % 3;
    float half = spacing / 2.0f;
    float minY = ceiling + half;
    float maxY = floor - (length - 1) * spacing - half;
    if (maxY < minY) maxY = minY;

    float spawnY = minY + static_cast<float>(rand() % static_cast<int>(maxY - minY + 1));
    createDiagonal(objects, spawnX, spawnY, spacing, length);
    return length * spacing;
}

float CoinFormation::createWaveRandom(std::vector<std::unique_ptr<Object>>& objects,
    float spawnX, float ceiling,
    float floor, float spacing)
{
    int   length = 12 + rand() % 6;
    float amplitude = spacing * 1.4f;
    float half = spacing / 2.0f;
    float minY = ceiling + amplitude + half;
    float maxY = floor - amplitude - half;
    if (maxY < minY) maxY = minY;

    float spawnY = minY + static_cast<float>(rand() % static_cast<int>(maxY - minY + 1));
    createWave(objects, spawnX, spawnY, spacing, length);
    return length * (spacing * 1.7f);
}

float CoinFormation::createTriangleRandom(std::vector<std::unique_ptr<Object>>& objects,
    float spawnX, float ceiling,
    float floor, float spacing)
{
    int   size = 3 + rand() % 3;
    float half = spacing / 2.0f;
    float minY = ceiling + half;
    float maxY = floor - (size - 1) * spacing - half;
    if (maxY < minY) maxY = minY;

    float spawnY = minY + static_cast<float>(rand() % static_cast<int>(maxY - minY + 1));
    createTriangle(objects, spawnX, spawnY, spacing, size);
    return size * spacing;
}

float CoinFormation::createCircleRandom(std::vector<std::unique_ptr<Object>>& objects,
    float spawnX, float ceiling,
    float floor, float spacing)
{
    int   radiusFactor = 2;
    float radius = spacing * 3.0f;
    float half = spacing / 2.0f;
    float minY = ceiling + radius + half;
    float maxY = floor - radius - half;
    if (maxY < minY) maxY = minY;

    float spawnY = minY + static_cast<float>(rand() % static_cast<int>(maxY - minY + 1));
    createCircle(objects, spawnX, spawnY, spacing, radiusFactor);
    return radius * 2.0f;
}


float CoinFormation::createRandom(std::vector<std::unique_ptr<Object>>& objects,
    float spawnX,
    float ceilingLimit,
    float floorLimit,
    float coinSpacing)
{
    using Creator = float(*)(std::vector<std::unique_ptr<Object>>&,
        float, float, float, float);

    static const Creator creators[] = {
        createLineRandom,
        createDiagonalRandom,
        createWaveRandom,
        createTriangleRandom,
        createCircleRandom
    };

    static const int count = static_cast<int>(std::size(creators));
    int choice = rand() % count;
    return creators[choice](objects, spawnX, ceilingLimit, floorLimit, coinSpacing);
}