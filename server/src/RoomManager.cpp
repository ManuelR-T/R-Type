/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#include "RoomManager.hpp"
#include "GameRunner.hpp"
#include "RTypeTCPProtol.hpp"

void rts::RoomManager::createRoom(const std::string &name, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPData::SER_ROOM_CREATED> packet{.cmd = rt::TCPCommand::SER_ROOM_CREATED};

    _rooms[name] = Room{};

    std::memcpy(packet.data.room_name, name.c_str(), name.size());
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::deleteRoom(const std::string &name, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPData::SER_ROOM_DELETED> packet{.cmd = rt::TCPCommand::SER_ROOM_DELETED};

    if (_rooms.at(name).player.empty()) {
        _rooms.erase(name);
        std::memcpy(packet.data.room_name, name.c_str(), name.size());
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

    std::memcpy(packet.data.room_name, name.c_str(), name.size());
    std::memcpy(packet.data.player_name, playerName.c_str(), playerName.size());
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::leaveRoom(const std::string &name, std::size_t playerId, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPData::SER_ROOM_LEAVED> packet{.cmd = rt::TCPCommand::SER_ROOM_LEAVED};

    std::memcpy(packet.data.room_name, name.c_str(), name.size());
    std::memcpy(
        packet.data.player_name,
        _rooms.at(name).player.at(playerId).name.c_str(),
        _rooms.at(name).player.at(playerId).name.size()
    );

    _rooms.at(name).player.erase(playerId);
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::playerReady(const std::string &roomName, std::size_t playerId, ntw::TCPServer &tcpServer)
{
    {
        rt::TCPPacket<rt::TCPData::SER_READY> packet{.cmd = rt::TCPCommand::SER_READY};

        std::memcpy(
            packet.data.player_name,
            _rooms.at(roomName).player.at(playerId).name.c_str(),
            _rooms.at(roomName).player.at(playerId).name.size()
        );
        std::memcpy(packet.data.room_name, roomName.c_str(), roomName.size());

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
        [](int port, std::promise<bool> serverReady) {
            rts::GameRunner gameRunner(port);
            serverReady.set_value(true);
            gameRunner.runGame();
        },
        _nextPort,
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
        std::memcpy(packet.data.room_name, roomName.c_str(), roomName.size());
        tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
}

void rts::RoomManager::playerNotReady(const std::string &roomName, std::size_t playerId, ntw::TCPServer &tcpServer)
{
    rt::TCPPacket<rt::TCPData::SER_NOT_READY> packet{.cmd = rt::TCPCommand::SER_NOT_READY};

    std::memcpy(
        packet.data.player_name,
        _rooms.at(roomName).player.at(playerId).name.c_str(),
        _rooms.at(roomName).player.at(playerId).name.size()
    );
    std::memcpy(packet.data.room_name, roomName.c_str(), roomName.size());

    _rooms.at(roomName).player.at(playerId).ready = false;
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::sendListRoom(std::size_t playerId, ntw::TCPServer &tcpServer)
{
    for (const auto &[name, content] : _rooms) {
        {
            rt::TCPPacket<rt::TCPData::SER_ROOM_LIST> packet{.cmd = rt::TCPCommand::SER_ROOM_LIST};

            std::memcpy(packet.data.room_name, name.c_str(), name.size());
            tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
        bool isFullReady = !content.player.empty();
        for (const auto &[_, player] : content.player) {
            {
                rt::TCPPacket<rt::TCPData::SER_ROOM_CONTENT> packet{.cmd = rt::TCPCommand::SER_ROOM_CONTENT};
                packet.data.ready = player.ready;
                std::memcpy(packet.data.player_name, player.name.c_str(), player.name.size());
                tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
            }
            if (!player.ready) {
                isFullReady = false;
            }
        }
        if (isFullReady) {
            rt::TCPPacket<rt::TCPData::SER_ROOM_IN_GAME> packet{.cmd = rt::TCPCommand::SER_ROOM_IN_GAME};
            std::memcpy(packet.data.room_name, name.c_str(), name.size());
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
