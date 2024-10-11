/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** fireRandomMissileAi
*/

#pragma once

#include <functional>
#include <list>
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"

namespace rts::ais {
entity_t fireRandomMissileAi(
    ecs::Registry &reg,
    entity_t e,
    std::list<rt::UDPServerPacket> &datasToSend,
    std::function<bool()> cond = nullptr
);
} // namespace rts::ais
