/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#pragma once

#include <array>
#include <map>
#include <string>
#include <utility>
#include "tcp/TCPServer.hpp"

namespace rts {
class RoomManager {
    private:
    struct PlayerLobby {
        std::string name;
        bool ready = false;
    };

    struct Room {
        std::map<std::size_t, PlayerLobby> player;
        std::unique_ptr<std::thread> game;
    };

    std::map<std::string, Room> _rooms;
    int _nextPort = 8081;

    public:
    RoomManager() = default;

    ~RoomManager()
    {
        for (const auto &[name, room] : _rooms) {
            if (room.game) {
                room.game->join();
            }
        }
    }

    void createRoom(const std::string &name, server::TCPServer &tcpServer);
    void deleteRoom(const std::string &name, server::TCPServer &tcpServer);
    void joinRoom(
        const std::string &name,
        std::size_t playerId,
        const std::string &playerName,
        server::TCPServer &tcpServer
    );
    void leaveRoom(const std::string &name, std::size_t playerId, server::TCPServer &tcpServer);
    void playerReady(const std::string &roomName, std::size_t playerId, server::TCPServer &tcpServer);
    void playerNotReady(const std::string &roomName, std::size_t playerId, server::TCPServer &tcpServer);
    void sendListRoom(std::size_t playerId, server::TCPServer &tcpServer);
};
} // namespace rts
