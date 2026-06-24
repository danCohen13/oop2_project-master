#include "ObjectCleaner.h"
#include <algorithm>

void ObjectCleaner::cleanup(std::vector<std::unique_ptr<Object>>& objects, float playerX) {
    float cullX = playerX - 400.0f;

    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [cullX](const std::unique_ptr<Object>& obj) {
                bool isOutOfScreen = (obj->getPosition().x + obj->getGlobalBounds().size.x) < cullX;

                bool isCollected = obj->isDisposed();

                return isOutOfScreen || isCollected;
            }),
        objects.end()
    );
}