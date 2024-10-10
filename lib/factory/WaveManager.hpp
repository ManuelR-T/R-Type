/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** WaveManager
*/

#pragma once

#include <functional>
#include <vector>
#include "Registry.hpp"
#include "entity.hpp"

/**
 * @class WaveManager
 * @namespace ecs
 * @brief Manages waves of entities in the ECS (Entity Component System).
 *
 * The WaveManager class is responsible for managing waves of entities, including
 * adding new waves, adding mobs to waves, checking if an entity is a mob, removing
 * entities, and spawning the next wave.
 */

namespace ecs {
class WaveManager {

    /**
     * @typedef MobSpawner
     * @brief A function type that spawns an entity in the ECS registry.
     */
    using MobSpawner = std::function<entity_t(ecs::Registry &)>;

    /**
     * @typedef Wave
     * @brief A vector of MobSpawner functions representing a wave of entities.
     */
    using Wave = std::vector<MobSpawner>;

    public:
    /**
     * @brief Constructs a new WaveManager object.
     */
    WaveManager();

    /**
     * @brief Default destructor for WaveManager.
     */
    ~WaveManager() = default;

    /**
     * @brief Adds a new wave to the WaveManager.
     * @return The ID of the newly added wave.
     */
    size_t addNewWave();
    /**
     * @brief Adds a new mob spawner to a specific wave.
     * @param waveId The ID of the wave to add the mob spawner to.
     * @param spawner The mob spawner function to add.
     */
    void addNewMob(size_t waveId, std::function<entity_t(ecs::Registry &)> spawner);

    /**
     * @brief Checks if a given entity is a mob.
     * @param entity The entity to check.
     * @return True if the entity is a mob, false otherwise.
     */
    bool isMob(entity_t entity);
    /**
     * @brief Removes an entity from the WaveManager.
     * @param entity The entity to remove.
     */
    void removeEntity(entity_t entity);

    /**
     * @brief Spawns the next wave of entities in the ECS registry.
     * @param reg The ECS registry to spawn the entities in.
     */
    void spawnNextWave(ecs::Registry &reg);

    /**
     * @brief Gets the number of waves managed by the WaveManager.
     * @return The number of waves.
     */
    size_t getNbWave() const
    {
        return _waves.size();
    }

    /**
     * @brief Checks if there are any entities currently managed by the WaveManager.
     * @return True if there are entities, false otherwise.
     */
    bool hasEntity()
    {
        return !_currEntities.empty();
    }

    /**
     * @brief Checks if all waves have been spawned.
     * @return True if all waves have been spawned, false otherwise.
     */
    bool isEnd() const
    {
        return _nextWave == _waves.size();
    }

    private:
    /**
     * @var _nextWave
     * @brief The index of the next wave to spawn.
     */
    size_t _nextWave;
    /**
     * @var _currEntities
     * @brief A vector of entities currently managed by the WaveManager.
     */
    std::vector<entity_t> _currEntities;
    /**
     * @var _waves
     * @brief A vector of waves of entities.
     */
    std::vector<Wave> _waves;
};
} // namespace ecs
