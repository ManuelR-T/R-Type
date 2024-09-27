/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#pragma once

#include "TCPClient.hpp"
#include "GameProtocol.hpp"
#include <memory>
#include <map>
#include <utility>

namespace rtc {

class room_manager {
    public:
    struct room_content_t {
        std::map<std::string, bool> player;
        bool joinable = true;
    };

    private:
    std::map<std::string, room_content_t> _rooms;
    std::string _current_room;
    client::TCPClient &_tcpClient;
    std::size_t _user_id;
    std::string _user_name;

    public:
    room_manager(client::TCPClient &tcpClient, std::size_t user_id, std::string user_name)
        : _tcpClient(tcpClient), _user_id(user_id), _user_name(std::move(user_name))
    {
    }

    const std::string &get_current_room() const;
    std::string &get_current_room();

    const std::map<std::string, bool> &get_current_room_player() const;

    const std::map<std::string, room_content_t> &get_rooms() const;

    std::map<std::string, room_content_t> &get_rooms();

    void ask_to_create_room(const std::string &room_name);
    void ask_to_delete_room(const std::string &room_name);
    void ask_to_join_room(const std::string &room_name);
    void ask_to_leave_room();
    void ask_to_be_ready();
    void ask_to_be_not_ready();
};

} // namespace rtc
