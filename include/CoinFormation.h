#pragma once
#include <vector>
#include <memory>
#include "Object.h"

class CoinFormation {
public:
    static void createLine(std::vector<std::unique_ptr<Object>>& objects, float startX, float startY, float spacing, int length, int thickness);
    static void createDiagonal(std::vector<std::unique_ptr<Object>>& objects, float startX, float startY, float spacing, int length);
    static void createWave(std::vector<std::unique_ptr<Object>>& objects, float startX, float startY, float spacing, int length);
    static void createTriangle(std::vector<std::unique_ptr<Object>>& objects, float startX, float startY, float spacing, int size);
    static void createCircle(std::vector<std::unique_ptr<Object>>& objects, float startX, float startY, float spacing, int radiusFactor);
};