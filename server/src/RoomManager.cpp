/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#include "RoomManager.hpp"
#include <cstddef>
#include "GameRunner.hpp"
#include "RTypeTCPProtol.hpp"

void rts::RoomManager::createRoom(const std::string &name, std::size_t stage, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPData::SER_ROOM_CREATED> packet{.cmd = rt::TCPCommand::SER_ROOM_CREATED};

    _rooms[name] = Room{.stage = stage};

    packet.data.stage = stage;
    name.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::deleteRoom(const std::string &name, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPData::SER_ROOM_DELETED> packet{.cmd = rt::TCPCommand::SER_ROOM_DELETED};

    if (_rooms.at(name).player.empty()) {
        _rooms.erase(name);
        name.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
        tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
}

void rts::RoomManager::joinRoom(
    const std::string &name,
    std::size_t playerId,
    const std::string &playerName,
    ntw::TCPServer &tcpServer
)
{
    rt::TCPPacket<rt::TCPData::SER_ROOM_JOINED> packet{.cmd = rt::TCPCommand::SER_ROOM_JOINED};

    _rooms.at(name).player[playerId] = PlayerLobby{.name = playerName, .ready = false};

    packet.data.user_id = playerId;
    name.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
    playerName.copy(packet.data.user_name, sizeof(packet.data.user_name) - 1);
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::leaveRoom(const std::string &name, std::size_t playerId, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPData::SER_ROOM_LEAVED> packet{.cmd = rt::TCPCommand::SER_ROOM_LEAVED};
    bool playerWasReady = _rooms.at(name).player.at(playerId).ready;

    packet.data.user_id = playerId;
    name.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);

    _rooms.at(name).player.erase(playerId);
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
    if (playerWasReady && _rooms.at(name).player.empty()) {
        _rooms.at(name).game->detach();
        _rooms.at(name).stopGame = true;
        deleteRoom(name, tcpServer);
    }
}

void rts::RoomManager::playerReady(const std::string &roomName, std::size_t playerId, ntw::TCPServer &tcpServer)
{
    {
        rt::TCPPacket<rt::TCPData::SER_READY> packet{.cmd = rt::TCPCommand::SER_READY};

        packet.data.user_id = playerId;
        roomName.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);

        _rooms.at(roomName).player.at(playerId).ready = true;
        tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
    // * check if all player in the room are ready
    bool allReady = true;

    for (const auto &[id, player] : _rooms.at(roomName).player) {
        if (!player.ready) {
            allReady = false;
        }
    }
    if (!allReady) {
        return;
    }

    std::promise<bool> serverReady;
    std::future<bool> server = serverReady.get_future();

    _rooms.at(roomName).game = std::make_unique<std::thread>(
        [](int port, std::size_t stage, bool &stopGame, std::promise<bool> serverReady) {
            rts::GameRunner gameRunner(port, stage);
            serverReady.set_value(true);
            gameRunner.runGame(stopGame);
        },
        _nextPort,
        _rooms.at(roomName).stage,
        std::ref(_rooms.at(roomName).stopGame),
        std::move(serverReady)
    );
    server.wait();

    {
        rt::TCPPacket<rt::TCPData::SER_ROOM_READY> packet{.cmd = rt::TCPCommand::SER_ROOM_READY};
        packet.data.port = this->_nextPort++;
        for (const auto &[id, player] : _rooms.at(roomName).player) {
            tcpServer.sendToUser(id, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
    }
    {
        rt::TCPPacket<rt::TCPData::SER_ROOM_IN_GAME> packet{.cmd = rt::TCPCommand::SER_ROOM_IN_GAME};
        roomName.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
        tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
}

void rts::RoomManager::playerNotReady(const std::string &roomName, std::size_t playerId, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPData::SER_NOT_READY> packet{.cmd = rt::TCPCommand::SER_NOT_READY};

    packet.data.user_id = playerId;
    roomName.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);

    _rooms.at(roomName).player.at(playerId).ready = false;
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::sendListRoom(std::size_t playerId, ntw::TCPServer &tcpServer)
{
    for (const auto &[name, content] : _rooms) {
        {
            rt::TCPPacket<rt::TCPData::SER_ROOM_LIST> packet{.cmd = rt::TCPCommand::SER_ROOM_LIST};

            packet.data.stage = content.stage;
            name.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
            tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
        bool isFullReady = !content.player.empty();
        for (const auto &[id, player] : content.player) {
            {
                rt::TCPPacket<rt::TCPData::SER_ROOM_CONTENT> packet{.cmd = rt::TCPCommand::SER_ROOM_CONTENT};
                packet.data.ready = player.ready;
                packet.data.user_id = id;
                player.name.copy(packet.data.user_name, sizeof(packet.data.user_name) - 1);
                tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
            }
            if (!player.ready) {
                isFullReady = false;
            }
        }
        if (isFullReady) {
            rt::TCPPacket<rt::TCPData::SER_ROOM_IN_GAME> packet{.cmd = rt::TCPCommand::SER_ROOM_IN_GAME};
            name.copy(packet.data.room_name, sizeof(packet.data.room_name) - 1);
            tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
    }
}

void rts::RoomManager::playerDisconnected(std::size_t playerId, ntw::TCPServer &tcpServer)
{
    for (const auto &[roomName, room] : this->_rooms) {
        for (const auto &[id, _] : room.player) {
            if (playerId == id) {
                this->leaveRoom(roomName, id, tcpServer);
                return;
            }
        }
    }
}
