/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SoundManager
*/

#include "SoundManager.hpp"
#include <iostream>

namespace ecs {

SoundManager::SoundManager() : _currentMusic(nullptr) {}

SoundManager::~SoundManager()
{
    stopAllSoundEffects();
    if (_currentMusic) {
        _currentMusic->stop();
    }
}

bool SoundManager::loadSoundBuffer(const std::string &id, const std::string &filepath)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filepath)) {
        std::cerr << "Failed to load sound buffer: " << filepath << std::endl;
        return false;
    }
    _soundBuffers.emplace(id, std::move(buffer));
    return true;
}

const sf::SoundBuffer &SoundManager::getSoundBuffer(const std::string &id) const
{
    auto it = _soundBuffers.find(id);
    if (it != _soundBuffers.end()) {
        return it->second;
    }
    throw std::runtime_error("SoundBuffer not found: " + id);
}

void SoundManager::playSoundEffect(const std::string &bufferId, float volume, bool loop)
{
    sf::Sound sound;
    try {
        sound.setBuffer(getSoundBuffer(bufferId));
    } catch (const std::exception &e) {
        std::cerr << "Error playing sound effect: " << e.what() << std::endl;
        return;
    }
    sound.setVolume(volume);
    sound.setLoop(loop);
    sound.play();
    _activeSounds.emplace_back(std::make_unique<sf::Sound>(std::move(sound)));

    _activeSounds.erase(
        std::remove_if(
            _activeSounds.begin(),
            _activeSounds.end(),
            [](const std::unique_ptr<sf::Sound> &s) { return s->getStatus() == sf::Sound::Stopped; }
        ),
        _activeSounds.end()
    );
}

void SoundManager::stopAllSoundEffects()
{
    for (auto &sound : _activeSounds) {
        sound->stop();
    }
    _activeSounds.clear();
}

bool SoundManager::loadMusic(const std::string &id, const std::string &filepath)
{
    auto music = std::make_shared<sf::Music>();
    if (!music->openFromFile(filepath)) {
        std::cerr << "Failed to load music: " << filepath << std::endl;
        return false;
    }
    _musics.emplace(id, std::move(music));
    return true;
}

void SoundManager::playMusic(const std::string &id, float volume, bool loop)
{
    if (_currentMusic) {
        _currentMusic->stop();
    }

    auto it = _musics.find(id);
    if (it != _musics.end()) {
        _currentMusic = it->second;
        _currentMusic->setVolume(volume);
        _currentMusic->setLoop(loop);
        _currentMusic->play();
    } else {
        std::cerr << "Music ID not found: " << id << std::endl;
    }
}

void SoundManager::pauseMusic()
{
    if (_currentMusic && _currentMusic->getStatus() == sf::Music::Playing) {
        _currentMusic->pause();
    }
}

void SoundManager::stopMusic()
{
    if (_currentMusic) {
        _currentMusic->stop();
    }
}

void SoundManager::setMusicVolume(float volume)
{
    if (_currentMusic) {
        _currentMusic->setVolume(volume);
    }
}

} // namespace ecs
