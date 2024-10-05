#pragma once

#include "EntityFactory.hpp"
#include "SpriteManager.hpp"
#include "udp/UDPClient.hpp"

namespace ecs {

class ClientEntityFactory : public EntityFactory {
    public:
    ClientEntityFactory(ecs::Registry &reg, SpriteManager &spriteManager, ntw::UDPClient &udpClient);

    protected:
    void addComponents(entity_t entity, const nlohmann::json &componentsJson, bool isShared, int x, int y) override;
    void handleNetworkSync(entity_t entity, const nlohmann::json &entityJson, bool isShared) override;

    private:
    SpriteManager &_spriteManager;
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
};

} // namespace ecs
