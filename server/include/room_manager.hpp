/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#pragma once

#include <string>
#include <unordered_map>

namespace rts {

struct room {
    std::string name;
    std::size_t nb_player = 0;
    
};

class room_manager
{
    private:
    std::unordered_map<std::string, room> _rooms;

    public:
    //    room_manager();
    //    ~room_manager();
};
} // namespace server
