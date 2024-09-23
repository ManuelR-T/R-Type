/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** missiles_stop
*/

#include "missiles_stop.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "core/zipper.hpp"

void ecs::systems::missiles_stop(ecs::registry &reg)
{
    auto &missiles = reg.get_components<ecs::component::missile>();
    auto &positions = reg.get_components<ecs::component::position>();

    size_t entityId = 0;
    for (size_t entityId = 0; entityId < missiles.size() && entityId < positions.size(); entityId++) {
        auto &pos = positions[entityId];
        auto &missile = missiles[entityId];
        if (missile && pos && (pos->x > missile->endx || pos->y > missile->endy)) {
            reg.kill_entity(entityId);
        }
    }
}
