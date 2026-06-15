#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class Resources {
public:
    // Destructive copy operations are disabled
    Resources(const Resources&) = delete;
    Resources& operator=(const Resources&) = delete;

    // Singleton global access point
    static Resources& getInstance();

    // Asset loaders
    void loadTexture(const std::string& name, const std::string& filename);
    void loadSound(const std::string& name, const std::string& filename);
    void loadFont(const std::string& filename);

    // Asset getters
    const sf::Texture& getTexture(const std::string& name) const;
    const sf::SoundBuffer& getSound(const std::string& name) const;
    const sf::Font& getFont() const;

private:
    Resources() = default; // Private constructor
    ~Resources() = default;

    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::SoundBuffer> m_sounds;
    sf::Font m_font;
};