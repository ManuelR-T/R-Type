/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_manager
*/

#pragma once

#include <memory>
#include "GameProtocol.hpp"
#include "argParser.hpp"
#include "room_manager.hpp"
#include "rtype_client.hpp"

#include <imgui.h>
#include <imgui-SFML.h>

namespace rtc {

class game_manager {
    private:
    std::string _ip;
    std::string _player_name;

    client::TCPClient _tcpClient;
    bool _in_lobby = true;
    std::size_t _user_id = 0;
    int _game_port = 0;
    rtc::room_manager _room_manager;

    std::shared_ptr<sf::RenderWindow> _window;

    void setup_tcp_connection();
    void launch_game();

    public:
    game_manager(const std::string &ip, int port, const std::string &player_name)
        : _ip(ip), _tcpClient(ip, port, sizeof(rt::tcp_packet)), _player_name(player_name),
          _user_id(ecs::generate_shared_entity_id()), _room_manager(_tcpClient, _user_id, player_name)
    {
    }

    ~game_manager()
    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::CL_DISCONNECT_USER};
        packet.body.cl_disconnect_user.user_id = _user_id;
        _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
        ImGui::SFML::Shutdown();
    }

    void run_game()
    {
        setup_tcp_connection();
        launch_game();
    }
};
} // namespace rtc
