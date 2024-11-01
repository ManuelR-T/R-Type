/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** check_out_of_range
*/

#pragma once

#include "Registry.hpp"
#include "WaveManager.hpp"

#include <list>

namespace ecs::systems {

void checkOutOfRange(ecs::Registry &reg, WaveManager &waveManager, std::list<std::vector<char>> &datasToSend);

} // namespace ecs::systems