/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeClient
*/

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <list>
#include "RoomManager.hpp"
#include "TickRateManager.hpp"
#include "core/InputManager.hpp"
#include "core/Registry.hpp"
#include "core/SpriteManager.hpp"
#include "udp/UDPClient.hpp"

namespace rtc {

void registerComponents(ecs::Registry &reg);
void registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::UDPClient &udpClient,
    ecs::InputManager &input,
    ntw::TickRateManager &tickRateManager,
    ecs::SpriteManager &spriteManager,
    std::list<std::function<void ()>> &_servModifierQueue
);

void createPlayer(ecs::Registry &reg, ntw::UDPClient &udpClient, ecs::SpriteManager &spriteManager);
void createStatic(ecs::Registry &reg, ecs::SpriteManager &spriteManager, float x, float y);
void createAi(ecs::Registry &reg, ecs::SpriteManager &spriteManager, float x, float y);
void createMissile(
    ecs::Registry &reg,
    ecs::SpriteManager &spriteManager,
    shared_entity_t sharedEntityId,
    float x,
    float y
);

void run(ecs::Registry &reg, const std::shared_ptr<sf::RenderWindow> &window, float &dt, ecs::InputManager &input);
void runGui(const std::shared_ptr<sf::RenderWindow> &window, rtc::RoomManager &roomManager, bool &inLobby);
} // namespace rtc
