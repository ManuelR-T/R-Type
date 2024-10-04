/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** setup_udp_connection
*/

#include "GameManager.hpp"

void rtc::GameManager::_setupUdpConnection(
    ecs::Registry &reg,
    ntw::UDPClient &udpClient,
    ecs::EntityFactory &entityFactory
)
{
    _registerUdpResponse(reg, entityFactory);
    udpClient.registerHandler([this](const char *data, std::size_t size) {
        _udpResponseHandler.handleResponse(data, size);
    });

    udpClient.run();
}
