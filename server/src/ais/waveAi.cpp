/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** waveAi
*/

#include "waveAi.hpp"
#include <cmath>
#include <cstdio>
#include "components/position.hpp"

void rts::ais::waveMovement(ecs::Registry &reg, entity_t e, float defaultY)
{
    if (reg.hasComponent<ecs::component::Position>(e)) {
        auto &pos = reg.getComponent<ecs::component::Position>(e);
        pos->y = defaultY + std::sin(pos->x) * 10;
    }
}
