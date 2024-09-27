/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#include "room_manager.hpp"

const std::string &rtc::room_manager::get_current_room() const
{
    return _current_room;
}

std::string &rtc::room_manager::get_current_room()
{
    return _current_room;
}

const std::map<std::string, bool> &rtc::room_manager::get_current_room_player() const
{
    return _rooms.at(_current_room).player;
}

const std::map<std::string, rtc::room_manager::room_content_t> &rtc::room_manager::get_rooms() const
{
    return _rooms;
}

std::map<std::string, rtc::room_manager::room_content_t> &rtc::room_manager::get_rooms()
{
    return _rooms;
}

void rtc::room_manager::ask_to_create_room(const std::string &room_name)
{
    rt::tcp_packet packet{.cmd = rt::tcp_command::CL_CREATE_ROOM};

    std::memcpy(packet.body.cl_create_room.room_name, room_name.c_str(), room_name.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::room_manager::ask_to_delete_room(const std::string &room_name)
{
    rt::tcp_packet packet{.cmd = rt::tcp_command::CL_DELETE_ROOM};

    std::memcpy(packet.body.cl_delete_room.room_name, room_name.c_str(), room_name.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::room_manager::ask_to_join_room(const std::string &room_name)
{
    rt::tcp_packet packet{.cmd = rt::tcp_command::CL_JOIN_ROOM};

    packet.body.cl_join_room.user_id = _user_id;
    std::memcpy(packet.body.cl_join_room.room_name, room_name.c_str(), room_name.size());
    std::memcpy(packet.body.cl_join_room.user_name, _user_name.c_str(), _user_name.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::room_manager::ask_to_leave_room()
{
    rt::tcp_packet packet{.cmd = rt::tcp_command::CL_LEAVE_ROOM};

    packet.body.cl_leave_room.user_id = _user_id;
    std::memcpy(packet.body.cl_leave_room.room_name, _current_room.c_str(), _current_room.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::room_manager::ask_to_be_ready()
{
    rt::tcp_packet packet{.cmd = rt::tcp_command::CL_READY};

    packet.body.cl_ready.user_id = _user_id;
    std::memcpy(packet.body.cl_ready.room_name, _current_room.c_str(), _current_room.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}

void rtc::room_manager::ask_to_be_not_ready()
{
    rt::tcp_packet packet{.cmd = rt::tcp_command::CL_NOT_READY};

    packet.body.cl_not_ready.user_id = _user_id;
    std::memcpy(packet.body.cl_not_ready.room_name, _current_room.c_str(), _current_room.size());
    _tcpClient.send(reinterpret_cast<const char *>(&packet), sizeof(packet));
}
