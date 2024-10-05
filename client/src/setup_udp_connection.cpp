/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** setup_udp_connection
*/

#include "GameManager.hpp"
#include "SpriteManager.hpp"

void rtc::GameManager::_setupUdpConnection(
    ecs::Registry &reg,
    ecs::SpriteManager &spriteManager,
    ntw::UDPClient &udpClient
)
{
    _registerUdpResponse(reg, spriteManager, udpClient);
    udpClient.registerHandler([this](const char *data, std::size_t size) {
        _udpResponseHandler.handleResponse(data, size);
    });

    udpClient.run();
}
