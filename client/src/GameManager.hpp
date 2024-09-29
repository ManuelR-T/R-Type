/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_manager
*/

#pragma once

#include <memory>
#include "RTypeTCPProtol.hpp"
#include "ResponseHandler.hpp"
#include "RoomManager.hpp"
#include "TCPClient.hpp"
#include "core/shared_entity.hpp"

#include <imgui.h>
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
    ntw::ResponseHandler<rt::TCPCommand, rt::TCPPacket> _tcpResponseHandler;

    std::shared_ptr<sf::RenderWindow> _window;

    void _registerTcpResponse();
    void _setupTcpConnection();
    void _launchGame();

    public:
    GameManager(const std::string &ip, int port, const std::string &playerName)
        : _ip(ip), _playerName(playerName), _tcpClient(ip, port, sizeof(rt::TCPPacket)),
          _userId(ecs::generateSharedEntityId()), _roomManager(_tcpClient, _userId, playerName),
          _tcpResponseHandler([](const rt::TCPPacket &packet) { return packet.cmd; })
    {
    }

    ~GameManager()
    {
        rt::TCPPacket packet{.cmd = rt::TCPCommand::CL_DISCONNECT_USER};
        packet.body.cl_disconnect_user.user_id = _userId;
        _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
        ImGui::SFML::Shutdown();
    }

    void runGame()
    {
        _setupTcpConnection();
        _launchGame();
    }
};
} // namespace rtc