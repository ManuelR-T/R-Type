/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#include "room_manager.hpp"
#include "game_runner.hpp"

void rts::RoomManager::createRoom(const std::string &name, server::TCPServer &tcpServer)
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::SER_ROOM_CREATED};

    _rooms[name] = Room{};

    std::memcpy(packet.body.ser_room_created.room_name, name.c_str(), name.size());
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::deleteRoom(const std::string &name, server::TCPServer &tcpServer)
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::SER_ROOM_DELETED};

    _rooms.erase(name);

    std::memcpy(packet.body.ser_room_deleted.room_name, name.c_str(), name.size());
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::joinRoom(
    const std::string &name,
    std::size_t playerId,
    const std::string &playerName,
    server::TCPServer &tcpServer
)
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::SER_ROOM_JOINED};

    _rooms.at(name).player[playerId] = PlayerLobby{.name = playerName, .ready = false};

    std::memcpy(packet.body.ser_room_joined.room_name, name.c_str(), name.size());
    std::memcpy(packet.body.ser_room_joined.player_name, playerName.c_str(), playerName.size());
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::leaveRoom(const std::string &name, std::size_t playerId, server::TCPServer &tcpServer)
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::SER_ROOM_LEAVED};

    std::memcpy(packet.body.ser_room_leaved.room_name, name.c_str(), name.size());
    std::memcpy(
        packet.body.ser_room_leaved.player_name,
        _rooms.at(name).player.at(playerId).name.c_str(),
        _rooms.at(name).player.at(playerId).name.size()
    );

    _rooms.at(name).player.erase(playerId);
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::playerReady(const std::string &roomName, std::size_t playerId, server::TCPServer &tcpServer)
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::SER_READY};
    bool allReady = true;

    std::memcpy(
        packet.body.ser_ready.player_name,
        _rooms.at(roomName).player.at(playerId).name.c_str(),
        _rooms.at(roomName).player.at(playerId).name.size()
    );
    std::memcpy(packet.body.ser_ready.room_name, roomName.c_str(), roomName.size());

    _rooms.at(roomName).player.at(playerId).ready = true;
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
    // * check if all player in the room are ready
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

    std::memset(reinterpret_cast<void *>(&packet), 0, sizeof(packet));
    packet.cmd = rt::TcpCommand::SER_ROOM_READY;
    packet.body.ser_room_ready.port = this->_nextPort++;
    for (const auto &[id, player] : _rooms.at(roomName).player) {
        tcpServer.sendToUser(id, reinterpret_cast<const char *>(&packet), sizeof(packet));
    }
    std::memset(reinterpret_cast<void *>(&packet), 0, sizeof(packet));
    packet.cmd = rt::TcpCommand::SER_ROOM_IN_GAME;
    std::memcpy(packet.body.ser_room_in_game.room_name, roomName.c_str(), roomName.size());
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::playerNotReady(const std::string &roomName, std::size_t playerId, server::TCPServer &tcpServer)
{
    rt::TcpPacket packet{.cmd = rt::TcpCommand::SER_NOT_READY};

    std::memcpy(
        packet.body.ser_not_ready.player_name,
        _rooms.at(roomName).player.at(playerId).name.c_str(),
        _rooms.at(roomName).player.at(playerId).name.size()
    );
    std::memcpy(packet.body.ser_not_ready.room_name, roomName.c_str(), roomName.size());

    _rooms.at(roomName).player.at(playerId).ready = false;
    tcpServer.sendToAllUser(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rts::RoomManager::sendListRoom(std::size_t playerId, server::TCPServer &tcpServer)
{
    for (const auto &[name, content] : _rooms) {
        {
            rt::TcpPacket packet{.cmd = rt::TcpCommand::SER_ROOM_LIST};

            std::memcpy(packet.body.ser_room_list.room_name, name.c_str(), name.size());
            tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
        bool isFullReady = !content.player.empty();
        for (const auto &[_, player] : content.player) {
            {
                rt::TcpPacket packet{.cmd = rt::TcpCommand::SER_ROOM_CONTENT};
                packet.body.ser_room_content.ready = player.ready;
                std::memcpy(packet.body.ser_room_content.player_name, player.name.c_str(), player.name.size());
                tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
            }
            if (!player.ready) {
                isFullReady = false;
            }
        }
        if (isFullReady) {
            rt::TcpPacket packet{.cmd = rt::TcpCommand::SER_ROOM_IN_GAME};
            std::memcpy(packet.body.ser_room_list.room_name, name.c_str(), name.size());
            tcpServer.sendToUser(playerId, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
    }
}
