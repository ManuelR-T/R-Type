#include "EntityFactory.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "components/controllable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "entity.hpp"
#include "components/share_movement.hpp"
#include "components/shared_entity.hpp"
#include "shared_entity.hpp"

namespace ecs {

EntityFactory::EntityFactory(ecs::Registry &reg, bool is_server) : _registry(reg), _is_server(is_server) {}

entity_t EntityFactory::createEntityFromJSON(
    const std::string &jsonFilePath,
    int x,
    int y,
    shared_entity_t sharedEntity
)
{
    std::ifstream file(jsonFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open entity JSON file: " + jsonFilePath);
    }

    nlohmann::json entityJson;
    file >> entityJson;

    std::string entityType = entityJson["type"].get<std::string>();
    bool isShared = entityType == "shared" || sharedEntity != std::numeric_limits<size_t>::max();
    std::cout << sharedEntity << std::endl;
    entity_t entity = 0;
    std::cout << "isShared:" << isShared << std::endl;
    if (isShared) {
        std::cout << "sharedEntity:" << sharedEntity << std::endl;
        std::cout << "std::numeric_limits<size_t>::max():" << std::numeric_limits<size_t>::max() << std::endl;
        if (sharedEntity == std::numeric_limits<size_t>::max()) {
            sharedEntity = generateSharedEntityId();
        }
        std::cout << "sharedEntity:" << sharedEntity << std::endl;
        if (_registry.getLocalEntity().contains(sharedEntity)) {
            entity = _registry.getLocalEntity().at(sharedEntity);
        } else {
            entity = _registry.spawnSharedEntity(sharedEntity);
        }
        std::cout << "Shared:" << sharedEntity << std::endl;
        std::cout << "entity:" << entity << std::endl;
    } else {
        entity = _registry.spawnEntity();
    }
    if (_registry.hasComponent<component::SharedEntity>(entity)) {
        std::cout << "s" << std::endl;
    } else {
        std::cout << "l" << std::endl;
    }
    std::cout << entity << std::endl;
    std::cout << "before addComponent" << std::endl;
    addComponents(entity, entityJson["components"], isShared, x, y);
    std::cout << "after addComponent" << std::endl;
    handleNetworkSync(entity, entityJson, isShared);

    return entity;
}

void EntityFactory::addCommonComponents(entity_t entity, const nlohmann::json &componentsJson, int x, int y)
{
    std::cout << "start addCommonComponent" << std::endl;
    if (componentsJson.contains("position")) {
        auto posJson = componentsJson["position"];
        if (x != INT32_MAX) {
            posJson["x"] = x;
        }
        if (y != INT32_MAX) {
            posJson["y"] = y;
        }
        _registry.addComponent(entity, ecs::component::Position{posJson["x"].get<float>(), posJson["y"].get<float>()});
    }

    if (componentsJson.contains("velocity")) {
        auto velJson = componentsJson["velocity"];
        _registry.addComponent(
            entity, ecs::component::Velocity{velJson["vx"].get<float>(), velJson["vy"].get<float>()}
        );
    }

    if (componentsJson.contains("hitbox")) {
        auto hitboxJson = componentsJson["hitbox"];
        _registry.addComponent(
            entity, ecs::component::Hitbox{hitboxJson["width"].get<float>(), hitboxJson["height"].get<float>()}
        );
    }
    std::cout << "middle addCommonComponent" << std::endl;
    if (componentsJson.contains("controllable")) {
        _registry.addComponent(entity, ecs::component::Controllable{});
    }

    if (componentsJson.contains("share_movement")) {
        _registry.addComponent(entity, ecs::component::ShareMovement{});
    }

    if (componentsJson.contains("missile")) {
        _registry.addComponent(entity, ecs::component::Missile{});
    }
    std::cout << "end addCommonComponent" << std::endl;
}

} // namespace ecs
