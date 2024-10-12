/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** horizontalMoveAi
*/

#include "horizontalMoveAi.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

void rts::ais::horizontalMoveAi(ecs::Registry &reg, entity_t e, float minX, float maxX)
{
    auto &pos = reg.getComponent<ecs::component::Position>(e);
    auto &vel = reg.getComponent<ecs::component::Velocity>(e);

    if (pos->x <= minX || pos->x >= maxX) {
        vel->vx *= -1;
    }
}
