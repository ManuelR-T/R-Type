/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** missiles_stop
*/

#include "missiles_stop.hpp"
#include <cstdio>

#include "IndexedZipper.hpp"
#include "RTypeConst.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"

void ecs::systems::missilesStop(ecs::Registry &reg)
{
    auto &missiles = reg.getComponents<ecs::component::Missile>();
    auto &positions = reg.getComponents<ecs::component::Position>();

    ecs::IndexedZipper<ecs::component::Position, ecs::component::Missile> zip(positions, missiles);

    for (auto [entityId, pos, missile] : zip) {
        if (pos.x < 0 || pos.y < 0 || pos.x > rt::SCREEN_WIDTH || pos.y > rt::SCREEN_HEIGHT) {
            reg.killEntity(entityId);
        }
    }
}
