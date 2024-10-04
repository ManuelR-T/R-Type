#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include "../components/animation.hpp"
#include "../components/velocity.hpp"
#include "../core/Registry.hpp"
#include "../core/SpriteManager.hpp"
#include "udp/UDPClient.hpp"
#include <unordered_map>
using json = nlohmann::json;

namespace ecs {

class EntityFactory {
    public:
    EntityFactory(ecs::Registry &reg, ecs::SpriteManager &spriteManager, ntw::UDPClient &udpClient);

    entity_t createEntityFromJSON(const std::string &jsonFilePath, int x = INT32_MAX, int y = INT32_MAX, entity_t = UINT_MAX);

    private:
    ecs::Registry &_registry;
    ecs::SpriteManager &_spriteManager;
    ntw::UDPClient &_udpClient;
    const std::unordered_map<std::string, std::function<void(Registry &, entity_t, ecs::component::Animation &)>>
        _animMap = {
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

    void addComponentsFromJSON(entity_t entity, const nlohmann::json &componentsJson, bool isShared = false, int x = INT32_MAX, int y = INT32_MAX);
};

} // namespace ecs
