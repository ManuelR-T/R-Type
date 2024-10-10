/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ClientEntityFactory
*/

#pragma once

#include "EntityFactory.hpp"
#include "SpriteManager.hpp"
#include "components/animation.hpp"
#include "udp/UDPClient.hpp"

namespace ecs {

class ClientEntityFactory : public EntityFactory {
    public:
    static void addComponents(
        Registry &reg,
        SpriteManager &spriteManager,
        entity_t entity,
        const nlohmann::json &componentsJson,
        bool isShared,
        int x,
        int y,
        float vx,
        float vy
    );
    static void handleNetworkSync(
        Registry &reg,
        ntw::UDPClient &udpClient,
        entity_t entity,
        const nlohmann::json &entityJson,
        bool isShared
    );

    private:
    static const std::unordered_map<std::string, std::function<void(Registry &, entity_t, ecs::component::Animation &)>>
        _animMap;
};

} // namespace ecs
