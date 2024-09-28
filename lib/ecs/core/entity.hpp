/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** entity
*/

#pragma once

#include <cstddef>
#include <queue>

/**
 * @typedef entity_t
 * @brief Alias for the entity identifier type.
 *
 * `entity_t` is defined as `std::size_t` to uniquely identify entities within the ECS.
 */
using entity_t = std::size_t;

namespace ecs {

/**
 * @class EntityManager
 * @brief Manages the creation and destruction of entities.
 *
 * The `EntityManager` class handles the allocation of unique entity identifiers and
 * reuses identifiers from destroyed entities to optimize resource usage.
 */
class EntityManager {
    public:
    /**
     * @brief Creates a new entity.
     *
     * This function generates a unique identifier for a new entity. If there are any
     * previously destroyed entities, their identifiers are reused. Otherwise, a new
     * identifier is generated by incrementing `next_entity_id`.
     *
     * @return A unique identifier of type `entity_t` for the newly created entity.
     */
    entity_t createEntity()
    {
        if (!_deadEntities.empty()) {
            entity_t id = _deadEntities.front();
            _deadEntities.pop();
            return id;
        }
        return _nextEntityId++;
    }

    /**
     * @brief Destroys an existing entity.
     *
     * This function marks an entity as destroyed by adding its identifier to the
     * `dead_entities` queue, making it available for reuse in future entity creations.
     *
     * @param entity The identifier of the entity to be destroyed.
     */
    void destroyEntity(entity_t entity)
    {
        _deadEntities.push(entity);
    }

    private:
    entity_t _nextEntityId = 0;        /**< The next available entity identifier */
    std::queue<entity_t> _deadEntities; /**< Queue of identifiers from destroyed entities */
};

} // namespace ecs
