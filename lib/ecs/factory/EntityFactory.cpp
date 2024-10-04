#include "EntityFactory.hpp"
#include <fstream>
#include <iostream>

#include "../components/animation.hpp"
#include "../components/controllable.hpp"
#include "../components/hitbox.hpp"
#include "../components/position.hpp"
#include "../components/sprite.hpp"
#include "../components/velocity.hpp"
#include "components/share_movement.hpp"
#include "RTypeUDPProtol.hpp"

namespace ecs {

EntityFactory::EntityFactory(ecs::Registry &reg, SpriteManager &spriteManager, ntw::UDPClient &udpClient)
    : _registry(reg), _spriteManager(spriteManager), _udpClient(udpClient)
{
}

entity_t EntityFactory::createEntityFromJSON(const std::string &jsonFilePath)
{
    std::cout << "Creating entity from JSON: " << jsonFilePath << std::endl;
    std::ifstream file(jsonFilePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open entity JSON file: " + jsonFilePath);
    }
    nlohmann::json entityJson;
    file >> entityJson;

    std::string entityType = entityJson["type"].get<std::string>();
    entity_t entity;
    bool isShared = entityType == "shared";
    if (isShared)
        entity = _registry.spawnSharedEntity(generateSharedEntityId());
    else
        entity = _registry.spawnEntity();
    addComponentsFromJSON(entity, entityJson["components"], isShared);
    if (_registry.getComponent<ecs::component::SharedEntity>(entity) != std::nullopt && isShared && entityJson.contains("network_command")) {
        std::cout << "Adding shared entity component" << std::endl;
        rt::UDPClientPacket msg = {
            .header = {.cmd = entityJson["network_command"].get<rt::UDPCommand>()},
            .body =
                {.sharedEntityId = _registry.getComponent<ecs::component::SharedEntity>(entity).value().sharedEntityId}
        };
        _udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    }
    return entity;
}

void EntityFactory::addComponentsFromJSON(entity_t entity, const nlohmann::json &componentsJson, bool isShared)
{
    if (componentsJson.contains("position")) {
        auto posJson = componentsJson["position"];
        _registry.addComponent(entity, ecs::component::Position{posJson["x"].get<float>(), posJson["y"].get<float>()});
    }

    if (componentsJson.contains("velocity")) {
        auto velJson = componentsJson["velocity"];
        _registry.addComponent(
            entity, ecs::component::Velocity{velJson["vx"].get<float>(), velJson["vy"].get<float>()}
        );
    }

    if (componentsJson.contains("sprite")) {
        auto spriteJson = componentsJson["sprite"];
        ecs::component::Sprite spriteComp;
        spriteComp.textureId = spriteJson["texture"].get<std::string>();
        spriteComp.spriteObj.setTexture(_spriteManager.getTexture(spriteComp.textureId));
        auto frameJson = spriteJson["initial_frame"];
        spriteComp.spriteObj.setTextureRect(sf::IntRect(
            frameJson["x"].get<int>(),
            frameJson["y"].get<int>(),
            frameJson["width"].get<int>(),
            frameJson["height"].get<int>()
        ));
        _registry.addComponent(entity, std::move(spriteComp));
    }

    if (componentsJson.contains("animation")) {
        auto animJson = componentsJson["animation"];
        ecs::component::Animation animComp;
        animComp.frameTime = animJson["frame_time"].get<float>();

        for (auto &[stateName, framesJson] : animJson["frames"].items()) {
            for (auto &frameJson : framesJson) {
                animComp.frames[stateName].emplace_back(
                    frameJson["x"].get<int>(),
                    frameJson["y"].get<int>(),
                    frameJson["width"].get<int>(),
                    frameJson["height"].get<int>()
                );
            }
        }
        if (animJson.contains("current_frame")) {
            animComp.currentFrame = animJson["current_frame"].get<size_t>();
        }
        if (animJson.contains("state")) {
            animComp.state = animJson["state"].get<std::string>();
        } else {
            animComp.state = "idle";
        }
        animComp.updateState = _animMap.at(animJson["update_state"].get<std::string>());

        _registry.addComponent(entity, std::move(animComp));
    }

    if (componentsJson.contains("hitbox")) {
        auto hitboxJson = componentsJson["hitbox"];
        _registry.addComponent(
            entity, ecs::component::Hitbox{hitboxJson["width"].get<float>(), hitboxJson["height"].get<float>()}
        );
    }

    if (componentsJson.contains("controllable")) {
        _registry.addComponent(entity, ecs::component::Controllable{});
    }

    if (componentsJson.contains("share_movement")) {
        _registry.addComponent(entity, ecs::component::ShareMovement{});
    }
}

} // namespace ecs
