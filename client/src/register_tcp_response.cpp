/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_tcp_response
*/

#include "game_manager.hpp"

void rtc::GameManager::_registerTcpResponse()
{
    _tcpResponseHandler.registerHandler(rt::TcpCommand::SER_ROOM_LIST, [this](const rt::TcpPacket &packet) {
        _roomManager.getRooms()[packet.body.ser_room_list.room_name] = {};
    });
    _tcpResponseHandler.registerHandler(rt::TcpCommand::SER_ROOM_CONTENT, [this](const rt::TcpPacket &packet) {
        _roomManager.getRooms().rbegin()->second.player[packet.body.ser_room_content.player_name] =
            packet.body.ser_room_content.ready;
    });
    _tcpResponseHandler.registerHandler(rt::TcpCommand::SER_ROOM_READY, [this](const rt::TcpPacket &packet) {
        _inLobby = false;
        _gamePort = packet.body.ser_room_ready.port;
    });
    _tcpResponseHandler.registerHandler(rt::TcpCommand::SER_ROOM_CREATED, [this](const rt::TcpPacket &packet) {
        _roomManager.getRooms()[packet.body.ser_room_created.room_name] = {};
    });
    _tcpResponseHandler.registerHandler(rt::TcpCommand::SER_ROOM_DELETED, [this](const rt::TcpPacket &packet) {
        _roomManager.getRooms().erase(packet.body.ser_room_deleted.room_name);
    });
    _tcpResponseHandler.registerHandler(rt::TcpCommand::SER_ROOM_JOINED, [this](const rt::TcpPacket &packet) {
        _roomManager.getRooms()
            .at(packet.body.ser_room_joined.room_name)
            .player[packet.body.ser_room_joined.player_name] = false;
        if (this->_playerName == packet.body.ser_room_joined.player_name) {
            _roomManager.getCurrentRoom() = packet.body.ser_room_joined.room_name;
        }
    });
    _tcpResponseHandler.registerHandler(rt::TcpCommand::SER_ROOM_LEAVED, [this](const rt::TcpPacket &packet) {
        _roomManager.getRooms()
            .at(packet.body.ser_room_leaved.room_name)
            .player.erase(packet.body.ser_room_leaved.player_name);
        if (this->_playerName == packet.body.ser_room_joined.player_name) {
            _roomManager.getCurrentRoom().clear();
        }
    });
    _tcpResponseHandler.registerHandler(rt::TcpCommand::SER_READY, [this](const rt::TcpPacket &packet) {
        _roomManager.getRooms().at(packet.body.ser_ready.room_name).player.at(packet.body.ser_ready.player_name) =
            true;
    });
    _tcpResponseHandler.registerHandler(rt::TcpCommand::SER_NOT_READY, [this](const rt::TcpPacket &packet) {
        _roomManager.getRooms()
            .at(packet.body.ser_not_ready.room_name)
            .player.at(packet.body.ser_not_ready.player_name) = false;
    });
    _tcpResponseHandler.registerHandler(rt::TcpCommand::SER_ROOM_IN_GAME, [this](const rt::TcpPacket &packet) {
        _roomManager.getRooms().at(packet.body.ser_room_in_game.room_name).joinable = false;
    });
}
