/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#pragma once

#include <array>
#include <future>
#include <map>
#include <string>
#include "TCPServer.hpp"
#include "game_runner.hpp"
#include <iostream>
#include <utility>

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
    ~room_manager()
    {
        for (const auto &[name, room] : _rooms) {
            if (room.game) {
                room.game->join();
            }
        }
    }

    void create_room(const std::string &name, std::size_t player_id, server::TCPServer &tcpServer)
    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::SER_ROOM_CREATED};

        _rooms[name] = room{};

        std::memcpy(packet.body.ser_room_created.room_name, name.c_str(), name.size());
        tcpServer.send_to_all_user(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }

    void delete_room(const std::string &name, std::size_t player_id, server::TCPServer &tcpServer)
    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::SER_ROOM_DELETED};

        _rooms.erase(name);

        std::memcpy(packet.body.ser_room_deleted.room_name, name.c_str(), name.size());
        tcpServer.send_to_all_user(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }

    void join_room(
        const std::string &name,
        std::size_t player_id,
        const std::string &player_name,
        server::TCPServer &tcpServer
    )
    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::SER_ROOM_JOINED};

        _rooms.at(name).player[player_id] = player_lobby{.name = player_name, .ready = false};

        std::memcpy(packet.body.ser_room_joined.room_name, name.c_str(), name.size());
        std::memcpy(packet.body.ser_room_joined.player_name, player_name.c_str(), player_name.size());
        tcpServer.send_to_all_user(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }

    void leave_room(const std::string &name, std::size_t player_id, server::TCPServer &tcpServer)
    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::SER_ROOM_LEAVED};

        std::memcpy(packet.body.ser_room_leaved.room_name, name.c_str(), name.size());
        std::memcpy(
            packet.body.ser_room_leaved.player_name,
            _rooms.at(name).player.at(player_id).name.c_str(),
            _rooms.at(name).player.at(player_id).name.size()
        );

        _rooms.at(name).player.erase(player_id);
        tcpServer.send_to_all_user(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }

    void player_ready(const std::string &room_name, std::size_t player_id, server::TCPServer &tcpServer)
    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::SER_READY};
        bool all_ready = true;

        std::memcpy(
            packet.body.ser_ready.player_name,
            _rooms.at(room_name).player.at(player_id).name.c_str(),
            _rooms.at(room_name).player.at(player_id).name.size()
        );
        std::memcpy(packet.body.ser_ready.room_name, room_name.c_str(), room_name.size());

        _rooms.at(room_name).player.at(player_id).ready = true;
        tcpServer.send_to_all_user(reinterpret_cast<const char *>(&packet), sizeof(packet));
        // * check if all player in the room are ready
        for (const auto &[id, player] : _rooms.at(room_name).player) {
            if (!player.ready) {
                all_ready = false;
            }
        }
        if (!all_ready) {
            return;
        }

        std::promise<bool> server_ready;
        std::future<bool> server = server_ready.get_future();

        _rooms.at(room_name).game = std::make_unique<std::thread>(
            [](int port, std::promise<bool> server_ready) {
                rts::game_runner game_runner(port);
                server_ready.set_value(true);
                game_runner.run_game();
            },
            _next_port, std::move(server_ready)
        );
        server.wait();

        std::memset(reinterpret_cast<void *>(&packet), 0, sizeof(packet));
        packet.cmd = rt::tcp_command::SER_ROOM_READY;
        packet.body.ser_room_ready.port = this->_next_port++;
        for (const auto &[id, player] : _rooms.at(room_name).player) {
            tcpServer.send_to_user(id, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
        std::memset(reinterpret_cast<void *>(&packet), 0, sizeof(packet));
        packet.cmd = rt::tcp_command::SER_ROOM_IN_GAME;
        std::memcpy(packet.body.ser_room_in_game.room_name, room_name.c_str(), room_name.size());
        tcpServer.send_to_all_user(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }

    void player_not_ready(const std::string &room_name, std::size_t player_id, server::TCPServer &tcpServer)
    {
        rt::tcp_packet packet{.cmd = rt::tcp_command::SER_NOT_READY};

        std::memcpy(
            packet.body.ser_not_ready.player_name,
            _rooms.at(room_name).player.at(player_id).name.c_str(),
            _rooms.at(room_name).player.at(player_id).name.size()
        );
        std::memcpy(packet.body.ser_not_ready.room_name, room_name.c_str(), room_name.size());

        _rooms.at(room_name).player.at(player_id).ready = false;
        tcpServer.send_to_all_user(reinterpret_cast<const char *>(&packet), sizeof(packet));
    }

    void send_list_room(std::size_t player_id, server::TCPServer &tcpServer)
    {
        std::cout << "\n\nSTART SENDING ROOM !!! PLAYER ID: " << player_id << "\n";
        for (const auto &[name, content] : _rooms) {
            {
                rt::tcp_packet packet{.cmd = rt::tcp_command::SER_ROOM_LIST};

                std::memcpy(packet.body.ser_room_list.room_name, name.c_str(), name.size());
                tcpServer.send_to_user(player_id, reinterpret_cast<const char *>(&packet), sizeof(packet));
                std::cout << "SEND ROOM !!!\n";
            }
            bool is_full_ready = !content.player.empty();
            for (const auto &[_, player] : content.player) {
                {
                    rt::tcp_packet packet{.cmd = rt::tcp_command::SER_ROOM_CONTENT};
                    packet.body.ser_room_content.ready = player.ready;
                    std::memcpy(packet.body.ser_room_content.player_name, player.name.c_str(), player.name.size());
                    tcpServer.send_to_user(player_id, reinterpret_cast<const char *>(&packet), sizeof(packet));
                    std::cout << "SEND ROOM CONTENT !!!\n";
                }
                if (!player.ready) {
                    is_full_ready = false;
                }
            }
            rt::tcp_packet packet{.cmd = rt::tcp_command::SER_ROOM_IN_GAME};
            std::memcpy(packet.body.ser_room_list.room_name, name.c_str(), name.size());
            tcpServer.send_to_user(player_id, reinterpret_cast<const char *>(&packet), sizeof(packet));
        }
    }
};
} // namespace rts
