#pragma once

#include <climits>
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
        int x = INT32_MAX,
        int y = INT32_MAX,
        shared_entity_t sharedEntity = std::numeric_limits<size_t>::max()
    );

    static entity_t createServerEntityFromJSON(
        Registry &reg,
        const std::string &jsonFilePath,
        int x = INT32_MAX,
        int y = INT32_MAX,
        shared_entity_t sharedEntity = std::numeric_limits<size_t>::max()
    );

    static void addCommonComponents(Registry &reg, entity_t entity, const nlohmann::json &componentsJson, int x, int y);
};

} // namespace ecs
