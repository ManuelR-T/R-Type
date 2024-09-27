/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_tcp_response
*/

#include "game_manager.hpp"

void rtc::game_manager::register_tcp_response()
{
    _tcp_response_handler.register_handler(rt::tcp_command::SER_ROOM_LIST, [this](const rt::tcp_packet &packet) {
        _room_manager.get_rooms()[packet.body.ser_room_list.room_name] = {};
    });
    _tcp_response_handler.register_handler(rt::tcp_command::SER_ROOM_CONTENT, [this](const rt::tcp_packet &packet) {
        _room_manager.get_rooms().rbegin()->second.player[packet.body.ser_room_content.player_name] =
            packet.body.ser_room_content.ready;
    });
    _tcp_response_handler.register_handler(rt::tcp_command::SER_ROOM_READY, [this](const rt::tcp_packet &packet) {
        _in_lobby = false;
        _game_port = packet.body.ser_room_ready.port;
    });
    _tcp_response_handler.register_handler(rt::tcp_command::SER_ROOM_CREATED, [this](const rt::tcp_packet &packet) {
        _room_manager.get_rooms()[packet.body.ser_room_created.room_name] = {};
    });
    _tcp_response_handler.register_handler(rt::tcp_command::SER_ROOM_DELETED, [this](const rt::tcp_packet &packet) {
        _room_manager.get_rooms().erase(packet.body.ser_room_deleted.room_name);
    });
    _tcp_response_handler.register_handler(rt::tcp_command::SER_ROOM_JOINED, [this](const rt::tcp_packet &packet) {
        _room_manager.get_rooms()
            .at(packet.body.ser_room_joined.room_name)
            .player[packet.body.ser_room_joined.player_name] = false;
        if (this->_player_name == packet.body.ser_room_joined.player_name) {
            _room_manager.get_current_room() = packet.body.ser_room_joined.room_name;
        }
    });
    _tcp_response_handler.register_handler(rt::tcp_command::SER_ROOM_LEAVED, [this](const rt::tcp_packet &packet) {
        _room_manager.get_rooms()
            .at(packet.body.ser_room_leaved.room_name)
            .player.erase(packet.body.ser_room_leaved.player_name);
        if (this->_player_name == packet.body.ser_room_joined.player_name) {
            _room_manager.get_current_room().clear();
        }
    });
    _tcp_response_handler.register_handler(rt::tcp_command::SER_READY, [this](const rt::tcp_packet &packet) {
        _room_manager.get_rooms().at(packet.body.ser_ready.room_name).player.at(packet.body.ser_ready.player_name) =
            true;
    });
    _tcp_response_handler.register_handler(rt::tcp_command::SER_NOT_READY, [this](const rt::tcp_packet &packet) {
        _room_manager.get_rooms()
            .at(packet.body.ser_not_ready.room_name)
            .player.at(packet.body.ser_not_ready.player_name) = false;
    });
    _tcp_response_handler.register_handler(rt::tcp_command::SER_ROOM_IN_GAME, [this](const rt::tcp_packet &packet) {
        _room_manager.get_rooms().at(packet.body.ser_room_in_game.room_name).joinable = false;
    });
}
