/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EntityFactory
*/

#include "EntityFactory.hpp"
#include <fstream>
#include <stdexcept>
#include "ClientEntityFactory.hpp"
#include "Registry.hpp"
#include "ServerEntityFactory.hpp"
#include "SpriteManager.hpp"
#include "components/beam.hpp"
#include "components/controllable.hpp"
#include "components/health.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/score.hpp"
#include "components/velocity.hpp"
#include "entity.hpp"
#include "udp/UDPClient.hpp"
#include "components/share_movement.hpp"
#include "shared_entity.hpp"

namespace ecs {

entity_t EntityFactory::createClientEntityFromJSON(
    Registry &reg,
    SpriteManager &spriteManager,
    ntw::UDPClient &udpClient,
    const std::string &jsonFilePath,
    int x,
    int y,
    shared_entity_t sharedEntity,
    float vx,
    float vy
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
    entity_t entity = 0;

    if (isShared) {
        if (sharedEntity == std::numeric_limits<size_t>::max()) {
            sharedEntity = generateSharedEntityId();
        }
        if (reg.getLocalEntity().contains(sharedEntity)) {
            entity = reg.getLocalEntity().at(sharedEntity);
        } else {
            entity = reg.spawnSharedEntity(sharedEntity);
        }
    } else {
        entity = reg.spawnEntity();
    }

    ClientEntityFactory::addComponents(reg, spriteManager, entity, entityJson["components"], isShared, x, y, vx, vy);

    return entity;
}

entity_t EntityFactory::createServerEntityFromJSON(
    Registry &reg,
    const std::string &jsonFilePath,
    int x,
    int y,
    shared_entity_t sharedEntity,
    float vx,
    float vy
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
    entity_t entity = 0;
    if (isShared) {
        if (sharedEntity == std::numeric_limits<size_t>::max()) {
            sharedEntity = generateSharedEntityId();
        }
        if (reg.getLocalEntity().contains(sharedEntity)) {
            entity = reg.getLocalEntity().at(sharedEntity); // should never happen
        } else {
            entity = reg.spawnSharedEntity(sharedEntity);
        }
    } else {
        entity = reg.spawnEntity();
    }
    ServerEntityFactory::addComponents(reg, entity, entityJson["components"], isShared, x, y, vx, vy);
    ServerEntityFactory::handleNetworkSync(reg, entity, entityJson, isShared);

    return entity;
}

void EntityFactory::addCommonComponents(
    Registry &reg,
    entity_t entity,
    const nlohmann::json &componentsJson,
    int x,
    int y,
    float vx,
    float vy
)
{
    if (componentsJson.contains("position")) {
        auto posJson = componentsJson["position"];
        if (x != std::numeric_limits<int>::max()) {
            posJson["x"] = x;
        }
        if (y != std::numeric_limits<int>::max()) {
            posJson["y"] = y;
        }
        reg.addComponent(entity, ecs::component::Position{posJson["x"].get<float>(), posJson["y"].get<float>()});
    }
    if (componentsJson.contains("velocity")) {
        auto velJson = componentsJson["velocity"];
        if (vx != std::numeric_limits<float>::max()) {
            velJson["vx"] = vx;
        }
        if (vy != std::numeric_limits<float>::max()) {
            velJson["vy"] = vy;
        }
        reg.addComponent(entity, ecs::component::Velocity{velJson["vx"].get<float>(), velJson["vy"].get<float>()});
    }

    if (componentsJson.contains("hitbox")) {
        auto hitboxJson = componentsJson["hitbox"];
        reg.addComponent(
            entity, ecs::component::Hitbox{hitboxJson["width"].get<float>(), hitboxJson["height"].get<float>()}
        );
    }
    if (componentsJson.contains("controllable")) {
        reg.addComponent(entity, ecs::component::Controllable{});
    }

    if (componentsJson.contains("share_movement")) {
        reg.addComponent(entity, ecs::component::ShareMovement{});
    }

    if (componentsJson.contains("missile")) {
        reg.addComponent(entity, ecs::component::Missile{});
    }

    if (componentsJson.contains("health")) {
        auto healthJson = componentsJson["health"];
        reg.addComponent(
            entity, ecs::component::Health{healthJson["maxHp"].get<int>(), healthJson["currHp"].get<int>()}
        );
    }
    if (componentsJson.contains("beam")) {
        float beamValue = componentsJson["beam"].get<float>();
        reg.addComponent(entity, ecs::component::Beam{beamValue});
    }
    if (componentsJson.contains("score")) {
        float scoreValue = componentsJson["score"].get<float>();
        ecs::component::Score score;
        sf::Font font;
        if (!font.loadFromFile("assets/font/DroidSansMono.ttf")) {
            return;
        }
        score.font = font;
        score.text.setString(std::to_string(scoreValue));
        reg.addComponent(entity, ecs::component::Score{score});
    }
}

} // namespace ecs
