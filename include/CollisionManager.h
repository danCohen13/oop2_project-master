#pragma once
#include <vector>
#include <memory>
#include "Player.h"
#include "Object.h"

class CollisionManager {
public:
    CollisionManager() = default;
    ~CollisionManager() = default;

    void handleCollisions(Player& player, std::vector<std::unique_ptr<Object>>& objects);
};