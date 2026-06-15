#include "CollisionManager.h"
#include <algorithm>

void CollisionManager::handleCollisions(Player& player, std::vector<std::unique_ptr<Object>>& objects) {
    sf::FloatRect playerBounds = player.getGlobalBounds();

    for (auto& obj : objects) {
        if (playerBounds.findIntersection(obj->getGlobalBounds()).has_value()) {
            player.collide(*obj); // Le Double Dispatch s'occupe de mettre le flag de l'objet à true
        }
    }
}