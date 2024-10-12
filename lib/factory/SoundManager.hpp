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
    /**
     * @brief Constructs a new SoundManager object.
     */
    SoundManager();

    /**
     * @brief Destroys the SoundManager object.
     */
    ~SoundManager();

    /**
     * @brief Loads a sound buffer from a file.
     *
     * @param id The identifier for the sound buffer.
     * @param filepath The path to the sound file.
     * @return true if the sound buffer was loaded successfully, false otherwise.
     */
    bool loadSoundBuffer(const std::string &id, const std::string &filepath);

    /**
     * @brief Retrieves a sound buffer by its identifier.
     *
     * @param id The identifier of the sound buffer.
     * @return const sf::SoundBuffer& The sound buffer associated with the given identifier.
     */
    const sf::SoundBuffer &getSoundBuffer(const std::string &id) const;

    /**
     * @brief Plays a sound effect.
     *
     * @param bufferId The identifier of the sound buffer to play.
     * @param volume The volume of the sound effect (default is 100.f).
     * @param loop Whether the sound effect should loop (default is false).
     */
    void playSoundEffect(const std::string &bufferId, float volume = 100.f, bool loop = false);

    /**
     * @brief Stops all currently playing sound effects.
     */
    void stopAllSoundEffects();

    /**
     * @brief Loads music from a file.
     *
     * @param id The identifier for the music.
     * @param filepath The path to the music file.
     * @return true if the music was loaded successfully, false otherwise.
     */
    bool loadMusic(const std::string &id, const std::string &filepath);

    /**
     * @brief Plays music.
     *
     * @param id The identifier of the music to play.
     * @param volume The volume of the music (default is 100.f).
     * @param loop Whether the music should loop (default is true).
     */
    void playMusic(const std::string &id, float volume = 100.f, bool loop = true);

    /**
     * @brief Pauses the currently playing music.
     */
    void pauseMusic();

    /**
     * @brief Stops the currently playing music.
     */
    void stopMusic();

    /**
     * @brief Sets the volume of the currently playing music.
     *
     * @param volume The volume to set.
     */
    void setMusicVolume(float volume);

    private:
    /**
     * @brief A map of sound buffer identifiers to their corresponding sound buffers.
     */
    std::unordered_map<std::string, sf::SoundBuffer> _soundBuffers;

    /**
     * @brief A vector of active sound effects.
     */
    std::vector<std::unique_ptr<sf::Sound>> _activeSounds;

    /**
     * @brief A map of music identifiers to their corresponding music objects.
     */
    std::unordered_map<std::string, std::shared_ptr<sf::Music>> _musics;

    /**
     * @brief The currently playing music.
     */
    std::shared_ptr<sf::Music> _currentMusic;
};

;

} // namespace ecs
