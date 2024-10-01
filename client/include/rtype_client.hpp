/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** rtype_client
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "UDPClient.hpp"
#include "core/SpriteManager.hpp"
#include "core/registry.hpp"
#include "core/input_manager.hpp"
#include "core/tick_rate_manager.hpp"
#include "room_manager.hpp"

namespace rtc {

void register_components(ecs::registry &reg);
void register_systems(
    ecs::registry &reg,
    sf::RenderWindow &window,
    float &dt,
    client::UDPClient &udpClient,
    ecs::input_manager &input,
    ecs::tick_rate_manager &tick_rate_manager,
    SpriteManager &sprite_manager
);

void create_player(ecs::registry &reg, client::UDPClient &udpClient, SpriteManager &sprite_manager);
void create_static(ecs::registry &reg, SpriteManager &sprite_manager, float x, float y);
void create_ai(ecs::registry &reg, SpriteManager &sprite_manager, float x, float y);

void run(ecs::registry &reg, const std::shared_ptr<sf::RenderWindow> &window, float &dt, ecs::input_manager &input);
void runGui(const std::shared_ptr<sf::RenderWindow> &window, rtc::room_manager &room_manager, bool &in_lobby);
} // namespace rtc
