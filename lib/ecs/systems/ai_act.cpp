/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ai_act
*/

#include "systems/ai_act.hpp"
#include "components/ai_actor.hpp"

void ecs::systems::ai_act(registry &reg)
{
    auto &actors = reg.get_components<ecs::component::ai_actor>();

    for (size_t entityId = 0; entityId < actors.size(); entityId++) {
        if (actors[entityId]) {
            actors[entityId]->act(reg, entityId);
        }
    }
}
