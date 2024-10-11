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

    shared_entity_t sharedId = ecs::generateSharedEntityId();
    auto rMissile = ecs::ServerEntityFactory::createServerEntityFromJSON(
        reg,
        "assets/missileBall.json",
        aiPos->x - 20, // Make the pos dependable on random
        aiPos->y + 10,
        sharedId
    );

    datasToSend.push_back(rt::UDPServerPacket(
        {.header = {.cmd = rt::UDPCommand::NEW_ENTITY},
         .body =
             {.sharedEntityId = sharedId,
              .b = {.newEntityData = {rt::EntityType::MISSILE_BALL, {{aiPos->x - 20, aiPos->y + 10}, {0, 0}}}}}}
    ));
    return rMissile;
}
