#include "Resources.h"
#include "GameException.h" 

Resources& Resources::getInstance() {
    static Resources instance;
    return instance;
}

void Resources::loadTexture(const std::string& name, const std::string& filename) {
    sf::Texture tex;
    // Si le chargement échoue, on lève une exception immédiatement
    if (!tex.loadFromFile(filename)) {
        throw GameException("Erreur critique : Impossible de charger l'image -> " + filename);
    }
    m_textures[name] = std::move(tex);
}

void Resources::loadSound(const std::string& name, const std::string& filename) {
    sf::SoundBuffer buffer;
    // Gestion des exceptions pour les fichiers audio (.wav, .ogg)
    if (!buffer.loadFromFile(filename)) {
        throw GameException("Erreur critique : Impossible de charger le son -> " + filename);
    }
    m_sounds[name] = std::move(buffer);
}

void Resources::loadFont(const std::string& filename) {
    // CORRECTION SFML 3.0 : Utilisation obligatoire de openFromFile() pour les polices
    if (!m_font.openFromFile(filename)) {
        throw GameException("Erreur critique : Impossible de charger la police -> " + filename);
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