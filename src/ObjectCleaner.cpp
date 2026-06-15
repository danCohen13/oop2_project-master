#include "ObjectCleaner.h"
#include <algorithm>

void ObjectCleaner::cleanup(std::vector<std::unique_ptr<Object>>& objects, float playerX) {
    // Calcul de la bordure gauche invisible de la caméra
    float cullX = playerX - 400.0f;

    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [cullX](const std::unique_ptr<Object>& obj) {
                // Condition 1 : L'objet est sorti de l'écran par la gauche (Optimisation mémoire)
                bool isOutOfScreen = (obj->getPosition().x + obj->getGlobalBounds().size.x) < cullX;

                // Condition 2 : L'objet a été collecté ou détruit par le joueur (Logique de gameplay)
                bool isCollected = obj->isDisposed();

                // On supprime si l'une ou l'autre des conditions est vraie
                return isOutOfScreen || isCollected;
            }),
        objects.end()
    );
}