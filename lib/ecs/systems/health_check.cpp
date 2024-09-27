/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** health_check
*/

#include "health_check.hpp"
#include "components/health.hpp"

void ecs::systems::health_check(registry &reg)
{
    auto &healths = reg.get_components<ecs::component::health>();

    for (size_t entityId = 0; entityId < healths.size(); entityId++) {
        if (healths[entityId]->curr_hp <= 0) {
            reg.kill_entity(entityId);
        }
    }
}
