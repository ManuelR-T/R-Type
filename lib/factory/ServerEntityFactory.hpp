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

class ServerEntityFactory : public EntityFactory {
    public:
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
    static void handleNetworkSync(Registry &reg, entity_t entity, const nlohmann::json &entityJson, bool isShared);
};

} // namespace ecs
