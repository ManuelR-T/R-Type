/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** horizontalMoveAi
*/

#pragma once

#include "Registry.hpp"

namespace rts::ais {
void horizontalMoveAi(ecs::Registry &reg, entity_t e, float minX, float maxX);
}
