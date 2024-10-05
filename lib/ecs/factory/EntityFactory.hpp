#pragma once

#include <climits>
#include <functional>
#include <nlohmann/json.hpp>
#include <string>
#include "../components/animation.hpp"
#include "../components/velocity.hpp"
#include "../core/Registry.hpp"
#include "../core/entity.hpp"
#include "core/shared_entity.hpp"
#include <unordered_map>

namespace ecs {

class EntityFactory {
    public:
    EntityFactory(ecs::Registry &reg, bool is_server = false);
    virtual ~EntityFactory() = default;

    entity_t createEntityFromJSON(
        const std::string &jsonFilePath,
        int x = INT32_MAX,
        int y = INT32_MAX,
        shared_entity_t sharedEntity = std::numeric_limits<size_t>::max()
    );

    protected:
    ecs::Registry &_registry;
    bool _is_server;

    virtual void addComponents(entity_t entity, const nlohmann::json &componentsJson, bool isShared, int x, int y) = 0;

    virtual void handleNetworkSync(entity_t entity, const nlohmann::json &entityJson, bool isShared) = 0;

    void addCommonComponents(entity_t entity, const nlohmann::json &componentsJson, int x, int y);
};

} // namespace ecs
