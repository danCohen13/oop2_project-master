#include "AudioManager.h"
#include "Resources.h"
#include "GameException.h"

AudioManager& AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::playSound(const std::string& name) {
    cleanStoppedSounds();

    if (m_isMuted) return;

    m_activeSounds.emplace_back(Resources::getInstance().getSound(name));
    m_activeSounds.back().play();
}

void AudioManager::playMusic(const std::string& filename, float volume) {
    if (!m_music.openFromFile(filename)) {
        throw GameException("Errorr : Impossible to charge the music -> " + filename);
    }
    m_music.setLooping(true); 
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