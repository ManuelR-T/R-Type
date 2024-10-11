/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** fireRandomMissileAi
*/

#include "fireRandomMissileAi.hpp"

#include <cstdio>
#include <cstdlib>
#include "ServerEntityFactory.hpp"
#include "components/hitbox.hpp"
#include "components/position.hpp"
#include "entity.hpp"
#include "shared_entity.hpp"

entity_t rts::ais::fireRandomMissileAi(
    ecs::Registry &reg,
    entity_t e,
    std::list<rt::UDPServerPacket> &datasToSend,
    std::function<bool()> cond
)
{
    if (cond != nullptr && !cond()) {
        return std::numeric_limits<size_t>::max();
    }
    // Use rand() temporarly
    if (rand() % 100 > 0) {
        return std::numeric_limits<size_t>::max();
    }
    auto aiPos = reg.getComponent<ecs::component::Position>(e);
    auto hitbox = reg.getComponent<ecs::component::Hitbox>(e);

    // Random value between -1 and 1
    auto xFactor = -1.f + (static_cast<float>(rand() % 100) / 50.f);
    auto yFactor = -1.f + (static_cast<float>(rand() % 100) / 50.f);

    shared_entity_t sharedId = ecs::generateSharedEntityId();
    auto missilePosX = (aiPos->x - (hitbox->width / 2.f)) + xFactor * 50;
    auto missilePosY = (aiPos->y - (hitbox->height / 2.f)) + yFactor * 50;
    auto rMissile = ecs::ServerEntityFactory::createServerEntityFromJSON(
        reg, "assets/missileBall.json", missilePosX, missilePosY, sharedId, xFactor * 150, yFactor * 150
    );

    datasToSend.push_back(rt::UDPServerPacket(
        {.header = {.cmd = rt::UDPCommand::NEW_ENTITY},
         .body =
             {.sharedEntityId = sharedId,
              .b =
                  {.newEntityData =
                       {rt::EntityType::MISSILE_BALL, {{missilePosX, missilePosY}, {xFactor * 150, yFactor * 150}}}}}}
    ));
    return rMissile;
}
