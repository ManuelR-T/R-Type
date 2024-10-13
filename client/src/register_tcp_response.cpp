/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_tcp_response
*/

#include <string>
#include "GameManager.hpp"
#include "RTypeTCPProtol.hpp"
#include "RoomManager.hpp"

void rtc::GameManager::_registerTcpResponse()
{
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_ROOM_LIST>(
        rt::TCPCommand::SER_ROOM_LIST,
        [this](const rt::TCPPacket<rt::TCPData::SER_ROOM_LIST> &packet) {
            _roomManager.getRooms()[packet.data.room_name] = {.stage = packet.data.stage};
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_ROOM_CONTENT>(
        rt::TCPCommand::SER_ROOM_CONTENT,
        [this](const rt::TCPPacket<rt::TCPData::SER_ROOM_CONTENT> &packet) {
            _roomManager.getRooms().rbegin()->second.player[packet.data.user_id] =
                rtc::RoomManager::Player{.name = packet.data.user_name, .ready = packet.data.ready};
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_ROOM_READY>(
        rt::TCPCommand::SER_ROOM_READY,
        [this](const rt::TCPPacket<rt::TCPData::SER_ROOM_READY> &packet) {
            _inLobby = false;
            _gamePort = packet.data.port;
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_ROOM_CREATED>(
        rt::TCPCommand::SER_ROOM_CREATED,
        [this](const rt::TCPPacket<rt::TCPData::SER_ROOM_CREATED> &packet) {
            _roomManager.getRooms()[packet.data.room_name] = {.stage = packet.data.stage};
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_ROOM_DELETED>(
        rt::TCPCommand::SER_ROOM_DELETED,
        [this](const rt::TCPPacket<rt::TCPData::SER_ROOM_DELETED> &packet) {
            _roomManager.getRooms().erase(packet.data.room_name);
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_ROOM_JOINED>(
        rt::TCPCommand::SER_ROOM_JOINED,
        [this](const rt::TCPPacket<rt::TCPData::SER_ROOM_JOINED> &packet) {
            _roomManager.getRooms().at(packet.data.room_name).player[packet.data.user_id] =
                rtc::RoomManager::Player{.name = packet.data.user_name, .ready = false};
            if (this->_userId == packet.data.user_id) {
                _roomManager.getCurrentRoom() = packet.data.room_name;
            }
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_ROOM_LEAVED>(
        rt::TCPCommand::SER_ROOM_LEAVED,
        [this](const rt::TCPPacket<rt::TCPData::SER_ROOM_LEAVED> &packet) {
            _roomManager.getRooms().at(packet.data.room_name).player.erase(packet.data.user_id);
            if (this->_userId == packet.data.user_id) {
                _roomManager.getCurrentRoom().clear();
            }
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_READY>(
        rt::TCPCommand::SER_READY,
        [this](const rt::TCPPacket<rt::TCPData::SER_READY> &packet) {
            _roomManager.getRooms().at(packet.data.room_name).player.at(packet.data.user_id).ready = true;
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_NOT_READY>(
        rt::TCPCommand::SER_NOT_READY,
        [this](const rt::TCPPacket<rt::TCPData::SER_NOT_READY> &packet) {
            _roomManager.getRooms().at(packet.data.room_name).player.at(packet.data.user_id).ready = false;
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_ROOM_IN_GAME>(
        rt::TCPCommand::SER_ROOM_IN_GAME,
        [this](const rt::TCPPacket<rt::TCPData::SER_ROOM_IN_GAME> &packet) {
            _roomManager.getRooms().at(packet.data.room_name).joinable = false;
        }
    );
    _tcpResponseHandler.registerHandler<rt::TCPData::SER_ALL_UDP_CONNECTION_READY>(
        rt::TCPCommand::SER_ALL_UDP_CONNECTION_READY,
        [this](const rt::TCPPacket<rt::TCPData::SER_ALL_UDP_CONNECTION_READY> &) {
            this->_allUDPClientReady.set_value(true);
        }
    );
}
