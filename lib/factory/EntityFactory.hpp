/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EntityFactory
*/

#pragma once

#include <climits>
#include <limits>
#include <nlohmann/json.hpp>
#include <string>
#include "Registry.hpp"
#include "SpriteManager.hpp"
#include "entity.hpp"
#include "udp/UDPClient.hpp"
#include "shared_entity.hpp"

namespace ecs {

class EntityFactory {
    public:
    static entity_t createClientEntityFromJSON(
        Registry &reg,
        SpriteManager &spriteManager,
        ntw::UDPClient &udpClient,
        const std::string &jsonFilePath,
        int x = std::numeric_limits<int>::max(),
        int y = std::numeric_limits<int>::max(),
        shared_entity_t sharedEntity = std::numeric_limits<size_t>::max(),
        float vx = std::numeric_limits<float>::max(),
        float vy = std::numeric_limits<float>::max()
    );

    static entity_t createServerEntityFromJSON(
        Registry &reg,
        const std::string &jsonFilePath,
        int x = std::numeric_limits<int>::max(),
        int y = std::numeric_limits<int>::max(),
        shared_entity_t sharedEntity = std::numeric_limits<size_t>::max(),
        float vx = std::numeric_limits<float>::max(),
        float vy = std::numeric_limits<float>::max()
    );

    static void addCommonComponents(
        Registry &reg,
        entity_t entity,
        const nlohmann::json &componentsJson,
        int x,
        int y,
        float vx,
        float vy
    );
};

} // namespace ecs
