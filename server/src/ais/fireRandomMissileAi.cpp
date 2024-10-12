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

static float randomFloatRange(float min, float max)
{
    auto r = rand() % 101;
    auto d = std::abs(max - min);

    if (d == 0)
        return min;
    return min + (static_cast<float>(r) / (100.f / d));
}

entity_t rts::ais::fireRandomMissileAi(
    ecs::Registry &reg,
    entity_t e,
    std::list<rt::UDPServerPacket> &datasToSend,
    std::function<bool()> cond,
    std::array<float, 2> randXRange,
    std::array<float, 2> randYRange
)
{
    if (cond != nullptr && !cond()) {
        return std::numeric_limits<size_t>::max();
    }
    // Use rand() temporarly
    if (rand() % 101 > 0) {
        return std::numeric_limits<size_t>::max();
    }
    auto aiPos = reg.getComponent<ecs::component::Position>(e);
    auto hitbox = reg.getComponent<ecs::component::Hitbox>(e);

    auto xFactor = randomFloatRange(randXRange[0], randXRange[1]);
    auto yFactor = randomFloatRange(randYRange[0], randYRange[1]);

    shared_entity_t sharedId = ecs::generateSharedEntityId();
    auto missilePosX = (aiPos->x - (hitbox->width / 2.f)) + xFactor * 20;
    auto missilePosY = (aiPos->y - (hitbox->height / 2.f)) + yFactor * 20;
    auto rMissile = ecs::ServerEntityFactory::createServerEntityFromJSON(
        reg, "assets/missileBall.json", missilePosX, missilePosY, sharedId, xFactor * 150, yFactor * 150
    );

    datasToSend.push_back(rt::UDPServerPacket(
        {.header = {.cmd = rt::UDPCommand::NEW_ENTITY},
         .body =
             {.sharedEntityId = sharedId,
              .b =
                  {.newEntityData =
                       {.type = rt::EntityType::MISSILE_BALL,
                        .moveData = {{missilePosX, missilePosY}, {xFactor * 150, yFactor * 150}}}}}}
    ));
    return rMissile;
}
