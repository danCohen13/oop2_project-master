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
    template <typename T>
    static void registerType(const std::string& name);

    static std::unique_ptr<Object> createObject(const std::string& name, const sf::Vector2f& position) {
        auto& registry = getRegistry();
        auto it = registry.find(name);

        if (it != registry.end()) {
            return it->second(position);
        }

        throw GameException("GameObjectFactory : Type d'objet introuvable ou non enregistre -> " + name);
    }

private:
    GameObjectFactory() = delete;

    using CreatorFunc = std::function<std::unique_ptr<Object>(const sf::Vector2f&)>;

    static std::map<std::string, CreatorFunc>& getRegistry() {
        static std::map<std::string, CreatorFunc> registry;
        return registry;
    }
};


template <typename T>
void GameObjectFactory::registerType(const std::string& name) {
    getRegistry()[name] = [](const sf::Vector2f& position) -> std::unique_ptr<Object> {
        return std::make_unique<T>(position);
        };
}