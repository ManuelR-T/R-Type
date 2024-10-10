/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** health_check
*/

#include "health_check.hpp"
#include "components/health.hpp"

// indexed zipper
void ecs::systems::healthCheck(Registry &reg)
{
    auto &healths = reg.getComponents<ecs::component::Health>();

    for (size_t entityId = 0; entityId < healths.size(); entityId++) {
        if (healths[entityId] && healths[entityId]->currHp <= 0) {
            reg.killEntity(entityId);
        }
    }
}
