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
        int y
    );
    static void handleNetworkSync(Registry &reg, entity_t entity, const nlohmann::json &entityJson, bool isShared);
};

} // namespace ecs
