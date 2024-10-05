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
    // EntityFactory(bool is_server = false);
    // virtual ~EntityFactory() = default;

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

    // protected:
    // bool _is_server;

    // virtual void addComponents(
    //     Registry &reg,
    //     entity_t entity,
    //     const nlohmann::json &componentsJson,
    //     bool isShared,
    //     int x,
    //     int y
    // ) = 0;

    // virtual void handleNetworkSync(Registry &reg, entity_t entity, const nlohmann::json &entityJson, bool isShared) =
    // 0;

    static void addCommonComponents(Registry &reg, entity_t entity, const nlohmann::json &componentsJson, int x, int y);
};

} // namespace ecs
