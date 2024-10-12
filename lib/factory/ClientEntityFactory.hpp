/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ClientEntityFactory
*/

#pragma once

#include "EntityFactory.hpp"
#include "SpriteManager.hpp"
#include "components/animation.hpp"

/**
 * @class ClientEntityFactory
 * @brief A factory class for creating and managing client-side entities in the ECS framework.
 *
 * This class provides static methods to add components to entities and handle network synchronization.
 * It also maintains a map of animation functions for entities.
 */

namespace ecs {

class ClientEntityFactory : public EntityFactory {
    public:
    /**
     * @brief Adds components to an entity.
     *
     * @param reg Reference to the registry.
     * @param spriteManager Reference to the sprite manager.
     * @param entity The entity to which components will be added.
     * @param componentsJson JSON object containing the components to be added.
     * @param isShared Boolean indicating if the entity is shared.
     * @param x The x-coordinate of the entity.
     * @param y The y-coordinate of the entity.
     * @param vx The x-velocity of the entity.
     * @param vy The y-velocity of the entity.
     */
    static void addComponents(
        Registry &reg,
        SpriteManager &spriteManager,
        entity_t entity,
        const nlohmann::json &componentsJson,
        bool isShared,
        int x,
        int y,
        float vx,
        float vy
    );

    private:
    /**
     * @var _animMap
     * @brief A map of animation functions for entities.
     *
     * This map associates animation names with functions that apply animations to entities.
     */
    static const std::unordered_map<std::string, std::function<void(Registry &, entity_t, ecs::component::Animation &)>>
        _animMap;
};
} // namespace ecs
