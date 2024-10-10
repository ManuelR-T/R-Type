/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** SoundManager
*/

#pragma once

#include <SFML/Audio.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace ecs {

/**
 * @brief Manages sound buffers and music playback.
 */
class SoundManager {
    public:
    SoundManager();
    ~SoundManager();

    bool loadSoundBuffer(const std::string &id, const std::string &filepath);
    const sf::SoundBuffer &getSoundBuffer(const std::string &id) const;
    void playSoundEffect(const std::string &bufferId, float volume = 100.f, bool loop = false);
    void stopAllSoundEffects();

    bool loadMusic(const std::string &id, const std::string &filepath);
    void playMusic(const std::string &id, float volume = 100.f, bool loop = true);
    void pauseMusic();
    void stopMusic();
    void setMusicVolume(float volume);

    private:
    std::unordered_map<std::string, sf::SoundBuffer> _soundBuffers;
    std::vector<std::unique_ptr<sf::Sound>> _activeSounds;

    std::unordered_map<std::string, std::shared_ptr<sf::Music>> _musics;
    std::shared_ptr<sf::Music> _currentMusic;
};

;

} // namespace ecs
