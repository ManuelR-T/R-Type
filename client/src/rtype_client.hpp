/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** rtype_client
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "RoomManager.hpp"
#include "TickRateManager.hpp"
#include "UDPClient.hpp"
#include "core/InputManager.hpp"
#include "core/Registry.hpp"
#include "core/SpriteManager.hpp"

namespace rtc {

void registerComponents(ecs::Registry &reg);
void registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    client::UDPClient &udpClient,
    ecs::InputManager &input,
    TickRateManager &tickRateManager,
    SpriteManager &spriteManager
);

void createPlayer(ecs::Registry &reg, client::UDPClient &udpClient, SpriteManager &spriteManager);
void createStatic(ecs::Registry &reg, SpriteManager &spriteManager, float x, float y);
void createAi(ecs::Registry &reg, SpriteManager &spriteManager, float x, float y);

void run(ecs::Registry &reg, const std::shared_ptr<sf::RenderWindow> &window, float &dt, ecs::InputManager &input);
void runGui(const std::shared_ptr<sf::RenderWindow> &window, rtc::RoomManager &roomManager, bool &inLobby);
} // namespace rtc
