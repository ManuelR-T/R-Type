#include "ClientEntityFactory.hpp"
#include "RTypeUDPProtol.hpp"
#include "SpriteManager.hpp"
#include "components/sprite.hpp"
#include "udp/UDPClient.hpp"

namespace ecs {

// ClientEntityFactory::ClientEntityFactory(SpriteManager &spriteManager, ntw::UDPClient &udpClient)
//     : EntityFactory(false), _spriteManager(spriteManager), _udpClient(udpClient)
// {
// }

const std::unordered_map<std::string, std::function<void(Registry &, entity_t, ecs::component::Animation &)>>
    ClientEntityFactory::_animMap = {
        {"player",
         [](ecs::Registry &reg, entity_t id, ecs::component::Animation &anim) {
             auto velOpt = reg.getComponent<ecs::component::Velocity>(id);
             if (!velOpt) {
                 return;
             }

             auto &vel = *velOpt;
             float vy = vel.vy;
             std::string &state = anim.state;

             if (vy > 0) {
                 if (state == "up" || state == "top") {
                     state = "top";
                 } else if (state == "idle") {
                     state = "up";
                 } else {
                     state = "idle";
                 }
             } else if (vy < 0) {
                 if (state == "down" || state == "bottom") {
                     state = "bottom";
                 } else if (state == "idle") {
                     state = "down";
                 } else {
                     state = "idle";
                 }
             } else {
                 if (state == "top") {
                     state = "up";
                 } else if (state == "bottom") {
                     state = "down";
                 } else {
                     state = "idle";
                 }
             }
         }},
        {"none", [](ecs::Registry &, entity_t, ecs::component::Animation &) {}},
};

void ClientEntityFactory::addComponents(
    Registry &reg,
    SpriteManager &spriteManager,
    entity_t entity,
    const nlohmann::json &componentsJson,
    bool isShared,
    int x,
    int y
)
{
    addCommonComponents(reg, entity, componentsJson, x, y);

    if (componentsJson.contains("sprite")) {
        auto spriteJson = componentsJson["sprite"];
        ecs::component::Sprite spriteComp;
        spriteComp.textureId = spriteJson["texture"].get<std::string>();
        spriteComp.spriteObj.setTexture(spriteManager.getTexture(spriteComp.textureId));
        auto frameJson = spriteJson["initial_frame"];
        spriteComp.spriteObj.setTextureRect(sf::IntRect(
            frameJson["x"].get<int>(),
            frameJson["y"].get<int>(),
            frameJson["width"].get<int>(),
            frameJson["height"].get<int>()
        ));
        reg.addComponent(entity, std::move(spriteComp));
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

        reg.addComponent(entity, std::move(animComp));
    }
}

void ClientEntityFactory::handleNetworkSync(
    Registry &reg,
    ntw::UDPClient &udpClient,
    entity_t entity,
    const nlohmann::json &entityJson,
    bool isShared
)
{
    if (isShared && entityJson.contains("network_command")) {
        rt::UDPClientPacket msg = {
            .header = {.cmd = entityJson["network_command"].get<rt::UDPCommand>()},
            .body = {.sharedEntityId = reg.getComponent<ecs::component::SharedEntity>(entity).value().sharedEntityId}
        };
        udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    }
}

} // namespace ecs
