/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** rtype_server
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "GameProtocol.hpp"
#include "core/registry.hpp"
#include "core/response_handler.hpp"
#include "room_manager.hpp"

namespace rts {

void register_components(ecs::registry &reg);
void register_systems(ecs::registry &reg, sf::RenderWindow &window, float &dt);
void register_udp_response(
    ecs::registry &reg,
    ecs::response_handler<rt::udp_command, rt::udp_packet> &response_handler
);
void register_tcp_response(
    rts::room_manager &room_manager,
    server::TCPServer &tcpServer,
    ecs::response_handler<rt::tcp_command, rt::tcp_packet> &response_handler
);

// ! will be replace by factory
void create_player(ecs::registry &reg, shared_entity_t shared_entity_id);
void create_static(ecs::registry &reg, float x, float y);
void create_missile(ecs::registry &reg, const rt::udp_packet &msg);
// !
} // namespace rts
