/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** WaveManager
*/

#pragma once

#include <functional>
#include <iostream>
#include <vector>
#include "Registry.hpp"
#include "entity.hpp"

namespace ecs {
class WaveManager {

    using MobSpawner = std::function<entity_t(ecs::Registry &)>;

    using Wave = std::vector<MobSpawner>;

    public:
    WaveManager();

    ~WaveManager() = default;

    size_t addNewWave();
    void addNewMob(size_t waveId, std::function<entity_t(ecs::Registry &)> spawner);

    bool isMob(entity_t entity);
    void removeEntity(entity_t entity);
    void spawnNextWave(ecs::Registry &reg);

    size_t getNbWave() const
    {
        return _waves.size();
    }

    bool hasEntity()
    {
        return !_currEntities.empty();
    }

    bool isEnd() const
    {
        return _nextWave == _waves.size();
    }

    private:
    size_t _nextWave;
    std::vector<entity_t> _currEntities;
    std::vector<Wave> _waves;
};
} // namespace ecs
