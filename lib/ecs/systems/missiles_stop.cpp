/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** missiles_stop
*/

#include "missiles_stop.hpp"

#include "components/missile.hpp"
#include "components/position.hpp"
#include "core/constants.hpp"

using namespace ecs::constants;

void ecs::systems::missiles_stop(ecs::registry &reg)
{
    auto &missiles = reg.get_components<ecs::component::missile>();
    auto &positions = reg.get_components<ecs::component::position>();

    size_t entityId = 0;
    for (size_t entityId = 0; entityId < missiles.size() && entityId < positions.size(); entityId++) {
        auto &pos = positions[entityId];
        auto &missile = missiles[entityId];
        if (missile && pos){
            if ((pos->x < 0 || pos->y < 0) || (pos->x > screen_width || pos->y > screen_height)) {
                reg.kill_entity(entityId);
            }
        }
    }
}
