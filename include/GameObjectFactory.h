#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>
#include <map>
#include <memory>
#include <functional>
#include "Object.h"
#include "GameException.h"

class GameObjectFactory {
public:
    // 1. DÉCLARATION de la méthode template (Définie tout en bas, en dehors de la classe)
    template <typename T>
    static void registerType(const std::string& name);

    // 2. DÉFINITION de la méthode standard (Directement dans la classe)
    static std::unique_ptr<Object> createObject(const std::string& name, const sf::Vector2f& position) {
        auto& registry = getRegistry();
        auto it = registry.find(name);

        if (it != registry.end()) {
            // On appelle la fonction lambda stockée
            return it->second(position);
        }

        throw GameException("GameObjectFactory : Type d'objet introuvable ou non enregistre -> " + name);
    }

private:
    GameObjectFactory() = delete;

    using CreatorFunc = std::function<std::unique_ptr<Object>(const sf::Vector2f&)>;

    // 3. DÉFINITION du Singleton de la Map (Directement dans la classe)
    static std::map<std::string, CreatorFunc>& getRegistry() {
        static std::map<std::string, CreatorFunc> registry;
        return registry;
    }
};

// ============================================================================
// DÉFINITIONS DES MÉTHODES TEMPLATES (En dehors de la classe, dans le fichier .h)
// ============================================================================

template <typename T>
void GameObjectFactory::registerType(const std::string& name) {
    getRegistry()[name] = [](const sf::Vector2f& position) -> std::unique_ptr<Object> {
        return std::make_unique<T>(position);
        };
}