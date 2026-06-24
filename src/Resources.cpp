#include "Resources.h"
#include "GameException.h" 

Resources& Resources::getInstance() {
    static Resources instance;
    return instance;
}

void Resources::loadTexture(const std::string& name, const std::string& filename) {
    sf::Texture tex;
    if (!tex.loadFromFile(filename)) {
        throw GameException("Critical error: Impossible to charge the image -> " + filename);
    }
    m_textures[name] = std::move(tex);
}

void Resources::loadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filename)) {
        throw GameException("Critical error: Impossible to charge the sound -> " + filename);
    }
    m_sounds[name] = std::move(buffer);
}

void Resources::loadFont(const std::string& filename) {
    if (!m_font.openFromFile(filename)) {
        throw GameException("Critical error: Impossible to charge the police -> " + filename);
    }
}

const sf::Texture& Resources::getTexture(const std::string& name) const {
    return m_textures.at(name);
}

const sf::SoundBuffer& Resources::getSound(const std::string& name) const {
    return m_sounds.at(name);
}

const sf::Font& Resources::getFont() const {
    return m_font;
}