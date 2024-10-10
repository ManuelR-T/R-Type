/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** waveAi
*/

#pragma once

#include "Registry.hpp"
#include "entity.hpp"

namespace rts::ais {
    void waveMovement(ecs::Registry &reg, entity_t e, float defaultY);
}