/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#pragma once

#include <array>
#include <future>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include "TCPServer.hpp"

namespace rts {
class room_manager {
    private:
    struct player_lobby {
        std::string name;
        bool ready = false;
    };

    struct room {
        std::map<std::size_t, player_lobby> player;
        std::unique_ptr<std::thread> game;
    };

    std::map<std::string, room> _rooms;
    int _next_port = 8081;

    public:
    room_manager() = default;

    ~room_manager()
    {
        for (const auto &[name, room] : _rooms) {
            if (room.game) {
                room.game->join();
            }
        }
    }

    void create_room(const std::string &name, std::size_t player_id, server::TCPServer &tcpServer);
    void delete_room(const std::string &name, std::size_t player_id, server::TCPServer &tcpServer);
    void join_room(
        const std::string &name,
        std::size_t player_id,
        const std::string &player_name,
        server::TCPServer &tcpServer
    );
    void leave_room(const std::string &name, std::size_t player_id, server::TCPServer &tcpServer);
    void player_ready(const std::string &room_name, std::size_t player_id, server::TCPServer &tcpServer);
    void player_not_ready(const std::string &room_name, std::size_t player_id, server::TCPServer &tcpServer);
    void send_list_room(std::size_t player_id, server::TCPServer &tcpServer);
};
} // namespace rts
