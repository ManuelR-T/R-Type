/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** setup_tcp_connection
*/

#include <iostream>
#include "GameProtocol.hpp"
#include "game_manager.hpp"

void rtc::game_manager::setup_tcp_connection()
{
    this->register_tcp_response();
    _tcpClient.register_handler([this](const char *data, std::size_t size) {
        _tcp_response_handler.handle_response(data, size);
    });

    _tcpClient.run();

    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::CL_NEW_USER};
        packet.body.cl_new_user.user_id = _user_id;
        _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::CL_ROOM_LIST};
        packet.body.cl_room_list.user_id = _user_id;
        _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
}
