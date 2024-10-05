#include "ClientEntityFactory.hpp"
#include "RTypeUDPProtol.hpp"
#include "components/sprite.hpp"

namespace ecs {

ClientEntityFactory::ClientEntityFactory(ecs::Registry &reg, SpriteManager &spriteManager, ntw::UDPClient &udpClient)
    : EntityFactory(reg, false), _spriteManager(spriteManager), _udpClient(udpClient)
{
}

void ClientEntityFactory::addComponents(
    entity_t entity,
    const nlohmann::json &componentsJson,
    bool isShared,
    int x,
    int y
)
{
    addCommonComponents(entity, componentsJson, x, y);

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
        if (animJson.contains("update_state")) {
            animComp.updateState = _animMap.at(animJson["update_state"].get<std::string>());
        }

        _registry.addComponent(entity, std::move(animComp));
    }
}

void ClientEntityFactory::handleNetworkSync(entity_t entity, const nlohmann::json &entityJson, bool isShared)
{
    if (isShared && entityJson.contains("network_command")) {
        rt::UDPClientPacket msg = {
            .header = {.cmd = entityJson["network_command"].get<rt::UDPCommand>()},
            .body =
                {.sharedEntityId = _registry.getComponent<ecs::component::SharedEntity>(entity).value().sharedEntityId}
        };
        _udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    }
}

} // namespace ecs
