/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** setup_tcp_connection
*/

#include <iostream>
#include "game_manager.hpp"

void rtc::game_manager::setup_tcp_connection()
{
    _tcpClient.register_handler([this](const char *data, std::size_t size) {
        rt::tcp_packet packet{};

        std::memcpy(&packet, data, sizeof(packet));
        switch (packet.cmd) {
            case rt::tcp_command::SER_ROOM_LIST:
                _room_manager.get_rooms()[packet.body.ser_room_list.room_name] = {};
                break;
            case rt::tcp_command::SER_ROOM_CONTENT:
                _room_manager.get_rooms().rbegin()->second.player[packet.body.ser_room_content.player_name] =
                    packet.body.ser_room_content.ready;
                break;
            case rt::tcp_command::SER_ROOM_READY:
                _in_lobby = false;
                _game_port = packet.body.ser_room_ready.port;
                break;
            case rt::tcp_command::SER_ROOM_CREATED:
                _room_manager.get_rooms()[packet.body.ser_room_created.room_name] = {};
                break;
            case rt::tcp_command::SER_ROOM_DELETED:
                _room_manager.get_rooms().erase(packet.body.ser_room_deleted.room_name);
                break;
            case rt::tcp_command::SER_ROOM_JOINED:
                _room_manager.get_rooms()
                    .at(packet.body.ser_room_joined.room_name)
                    .player[packet.body.ser_room_joined.player_name] = false;
                if (this->_player_name == packet.body.ser_room_joined.player_name) {
                    _room_manager.get_current_room() = packet.body.ser_room_joined.room_name;
                }
                break;
            case rt::tcp_command::SER_ROOM_LEAVED:
                _room_manager.get_rooms()
                    .at(packet.body.ser_room_leaved.room_name)
                    .player.erase(packet.body.ser_room_leaved.player_name);
                if (this->_player_name == packet.body.ser_room_joined.player_name) {
                    _room_manager.get_current_room().clear();
                }
                break;
            case rt::tcp_command::SER_READY:
                _room_manager.get_rooms()
                    .at(packet.body.ser_ready.room_name)
                    .player.at(packet.body.ser_ready.player_name) = true;
                break;
            case rt::tcp_command::SER_NOT_READY:
                _room_manager.get_rooms()
                    .at(packet.body.ser_not_ready.room_name)
                    .player.at(packet.body.ser_not_ready.player_name) = false;
                break;
            case rt::tcp_command::SER_ROOM_IN_GAME:
                _room_manager.get_rooms().at(packet.body.ser_room_in_game.room_name).joinable = false;
                break;
        }
    });

    _tcpClient.run();

    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::CL_NEW_USER};
        packet.body.cl_new_user.user_id = _user_id;
        _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
    // ! wait for server ready
    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::CL_ROOM_LIST};
        packet.body.cl_room_list.user_id = _user_id;
        _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
}
