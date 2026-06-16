#include "AudioManager.h"
#include "Resources.h"
#include "GameException.h"

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::playSound(const std::string& name) {
    // Élimine de la liste les sons qui ont fini de jouer
    cleanStoppedSounds();

    if (m_isMuted) return;

    // SFML 3.0 réclame le buffer directement à l'emplacement mémoire (emplace_back)
    m_activeSounds.emplace_back(Resources::getInstance().getSound(name));
    m_activeSounds.back().play();
}

void AudioManager::playMusic(const std::string& filename, float volume) {
    if (!m_music.openFromFile(filename)) {
        throw GameException("Erreur : Impossible de charger la musique -> " + filename);
    }
    m_music.setLooping(true); // Norme SFML 3.0
    m_music.setVolume(m_isMuted ? 0.f : volume);
    m_music.play();
}

void AudioManager::stopMusic() {
    m_music.stop();
}

void AudioManager::setMuted(bool muted) {
    m_isMuted = muted;
    if (m_isMuted) {
        m_music.setVolume(0.f);
        for (auto& sound : m_activeSounds) {
            sound.stop();
        }
        m_activeSounds.clear();
    }
    else {
        m_music.setVolume(35.f);
    }
}

bool AudioManager::isMuted() const {
    return m_isMuted;
}

void AudioManager::cleanStoppedSounds() {
    m_activeSounds.remove_if([](const sf::Sound& sound) {
        return sound.getStatus() == sf::Sound::Status::Stopped;
        });
}