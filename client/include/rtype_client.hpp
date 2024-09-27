/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** rtype_client
*/

#pragma once

#include "TCPClient.hpp"
#include "UDPClient.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "core/constants.hpp"
#include "core/registry.hpp"
#include "systems/collision.hpp"
#include "systems/control.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "components/share_movement.hpp"
#include "components/shared_entity.hpp"
#include "core/input_manager.hpp"
#include "core/shared_entity.hpp"
#include "core/tick_rate_manager.hpp"
#include "room_manager.hpp"
#include "systems/control_special.hpp"
#include "systems/missiles_stop.hpp"
#include "systems/share_movement.hpp"

namespace rtc {

void register_components(ecs::registry &reg);
void register_systems(
    ecs::registry &reg,
    sf::RenderWindow &window,
    float &dt,
    client::UDPClient &udpClient,
    ecs::input_manager &input,
    ecs::tick_rate_manager &tick_rate_manager
);

void create_player(ecs::registry &reg, client::UDPClient &udpClient);
void create_static(ecs::registry &reg, float x, float y);

void run(
    ecs::registry &reg,
    const std::shared_ptr<sf::RenderWindow> &window,
    float &dt,
    client::UDPClient &udpClient,
    ecs::input_manager &input
);
void runGui(const std::shared_ptr<sf::RenderWindow> &window, rtc::room_manager &room_manager, bool &in_lobby);
} // namespace rtc