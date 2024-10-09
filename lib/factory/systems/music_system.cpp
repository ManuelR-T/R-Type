/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** music_system
*/

#include "music_system.hpp"
#include "components/music_component.hpp"

namespace ecs::systems {

void MusicSystem(ecs::Registry &reg, ecs::SoundManager &soundManager, float dt)
{
    auto &musicComponents = reg.getComponents<ecs::component::MusicComponent>();

    for (size_t i = 0; i < musicComponents.size(); ++i) {
        if (!musicComponents.has(i))
            continue;

        auto &musicComp = musicComponents[i];

        if (musicComp->isPlaying) {
            soundManager.playMusic(musicComp->musicFilePath, musicComp->volume, musicComp->loop);
            musicComp->isPlaying = false;
        }
    }
}

} // namespace ecs::systems
