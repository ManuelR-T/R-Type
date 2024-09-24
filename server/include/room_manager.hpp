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

namespace rts {

struct room_player {
    std::string name;
    bool ready;
};

struct room {
    std::string name;
    std::map<std::size_t, room_player> player;
};

class room_manager {
    private:
    std::map<std::string, room> _rooms;

    public:
    void create_room(const std::string &name)
    {
        _rooms[name] = room{.name = name};
    }

    void delete_room(const std::string &name)
    {
        _rooms.erase(name);
    }

    void join_room(const std::string &name,
        std::size_t player_id, const rts::room_player &player)
    {
        _rooms.at(name).player[player_id] = player;
    }

    void leave_room(const std::string &name, std::size_t player_id)
    {
        _rooms.at(name).player.erase(player_id);
    }

    void leave_game(const std::string &room_name, std::size_t player_id)
    {
        _rooms.at(room_name).player.erase(player_id);
    }
};
} // namespace rts
