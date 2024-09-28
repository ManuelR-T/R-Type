/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** missiles_stop
*/

#include "missiles_stop.hpp"

#include "components/missile.hpp"
#include "components/position.hpp"
#include "rtype_const.hpp"

void ecs::systems::missilesStop(ecs::Registry &reg)
{
    auto &missiles = reg.getComponents<ecs::component::Missile>();
    auto &positions = reg.getComponents<ecs::component::Position>();

    size_t entityId = 0;
    for (size_t entityId = 0; entityId < missiles.size() && entityId < positions.size(); entityId++) {
        auto &pos = positions[entityId];
        auto &missile = missiles[entityId];
        if (missile && pos) {
            if ((pos->x < 0 || pos->y < 0) || (pos->x > rt::SCREEN_WIDTH || pos->y > rt::SCREEN_HEIGHT)) {
                reg.killEntity(entityId);
            }
        }
    }
}
