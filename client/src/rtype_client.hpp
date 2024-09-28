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

void registerComponents(ecs::Registry &reg);
void registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    client::UDPClient &udpClient,
    ecs::InputManager &input,
    ecs::TickRateManager &tickRateManager,
    SpriteManager &spriteManager
);

void createPlayer(ecs::Registry &reg, client::UDPClient &udpClient, SpriteManager &spriteManager);
void createStatic(ecs::Registry &reg, SpriteManager &spriteManager, float x, float y);
void createAi(ecs::Registry &reg, SpriteManager &spriteManager, float x, float y);

void run(ecs::Registry &reg, const std::shared_ptr<sf::RenderWindow> &window, float &dt, ecs::InputManager &input);
void runGui(const std::shared_ptr<sf::RenderWindow> &window, rtc::RoomManager &roomManager, bool &inLobby);
} // namespace rtc
