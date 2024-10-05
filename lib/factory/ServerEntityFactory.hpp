#pragma once

#include "EntityFactory.hpp"

namespace ecs {

class ServerEntityFactory : public EntityFactory {
    public:
    ServerEntityFactory(ecs::Registry &reg);

    protected:
    void addComponents(entity_t entity, const nlohmann::json &componentsJson, bool isShared, int x, int y) override;
    void handleNetworkSync(entity_t entity, const nlohmann::json &entityJson, bool isShared) override;
};

} // namespace ecs
