/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ServerEntityFactory
*/

#pragma once

#include "EntityFactory.hpp"
#include "Registry.hpp"

namespace ecs {

/**
 * @class ServerEntityFactory
 * @brief A factory class for creating and managing server-side entities.
 */
class ServerEntityFactory : public EntityFactory {
    public:
    /**
     * @brief Adds components to an entity in the registry.
     *
     * @param reg The registry where the entity and its components are stored.
     * @param entity The entity to which components will be added.
     * @param componentsJson A JSON object containing the components to be added.
     * @param isShared A boolean indicating whether the entity is shared across the network.
     * @param x The x-coordinate position of the entity.
     * @param y The y-coordinate position of the entity.
     * @param vx The velocity of the entity along the x-axis.
     * @param vy The velocity of the entity along the y-axis.
     */
    static void addComponents(
        Registry &reg,
        entity_t entity,
        const nlohmann::json &componentsJson,
        bool isShared,
        int x,
        int y,
        float vx,
        float vy
    );
    /**
     * @brief Handles the synchronization of an entity's state with the network.
     *
     * @param reg The registry where the entity is stored.
     * @param entity The entity to be synchronized.
     * @param entityJson A JSON object containing the entity's state information.
     * @param isShared A boolean indicating whether the entity is shared across the network.
     */
    static void handleNetworkSync(Registry &reg, entity_t entity, const nlohmann::json &entityJson, bool isShared);
};

} // namespace ecs
