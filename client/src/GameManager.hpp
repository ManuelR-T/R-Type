/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_manager
*/

#pragma once

#include <functional>
#include <list>
#include <memory>
#include "Registry.hpp"
#include "RoomManager.hpp"
#include "SpriteManager.hpp"
#include "TCPResponseHandler.hpp"
#include "UDPResponseHandler.hpp"
#include "imgui.h"
#include "tcp/TCPClient.hpp"
#include "udp/UDPClient.hpp"
#include "shared_entity.hpp"
#include <imgui-SFML.h>

namespace rtc {

class GameManager {
    private:
    std::string _ip;
    std::string _playerName;

    ntw::TCPClient _tcpClient;
    bool _inLobby = true;
    std::size_t _userId = 0;
    int _gamePort = 0;
    rtc::RoomManager _roomManager;
    rt::TCPResponseHandler _tcpResponseHandler;
    rt::UDPResponseHandler _udpResponseHandler;

    std::list<std::function<void(ecs::Registry &reg)>> _networkCallbacks;
    std::shared_ptr<sf::RenderWindow> _window;
    ImFont *_font;

    void _registerTcpResponse();
    void _registerUdpResponse(ecs::Registry &reg, ecs::SpriteManager &spriteManager, ntw::UDPClient &udpClient);

    void _setupTcpConnection();
    void _setupUdpConnection(ecs::Registry &reg, ecs::SpriteManager &spriteManager, ntw::UDPClient &udpClient);
    void _launchGame();

    public:
    GameManager(const std::string &ip, int port, const std::string &playerName)
        : _ip(ip), _playerName(playerName), _tcpClient(ip, port), _userId(ecs::generateSharedEntityId()),
          _roomManager(_tcpClient, _userId, playerName)
    {
    }

    ~GameManager()
    {
        ImGui::SFML::Shutdown();
    }

    void runGame()
    {
        _setupTcpConnection();
        _launchGame();
    }
};
} // namespace rtc
