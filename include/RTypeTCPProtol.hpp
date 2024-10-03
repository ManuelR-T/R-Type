/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** RTypeTCPProtol
*/

#pragma once

#include <cstddef>
#include "RTypeConst.hpp"

namespace rt {
enum class TCPCommand : std::size_t {
    NONE,

    CL_NEW_USER,
    CL_DISCONNECT_USER,

    CL_CREATE_ROOM,
    SER_ROOM_CREATED,

    CL_DELETE_ROOM,
    SER_ROOM_DELETED,

    CL_JOIN_ROOM,
    SER_ROOM_JOINED,

    CL_LEAVE_ROOM,
    SER_ROOM_LEAVED,

    CL_READY,
    SER_READY,

    CL_NOT_READY,
    SER_NOT_READY,

    CL_ROOM_LIST,     // ask for room list
    SER_ROOM_LIST,    // send room list (one by one)
    SER_ROOM_CONTENT, // send room content (one by one)

    SER_ROOM_IN_GAME,
    SER_ROOM_READY
};
} // namespace rt

namespace rt::TCPData {
struct CL_NEW_USER {
    std::size_t user_id = 0;
};

struct CL_DISCONNECT_USER {
    std::size_t user_id = 0;
};

struct CL_CREATE_ROOM {
    char room_name[32] = {0};
};

struct SER_ROOM_CREATED {
    char room_name[32] = {0};
};

struct CL_DELETE_ROOM {
    char room_name[32] = {0};
};

struct SER_ROOM_DELETED {
    char room_name[32] = {0};
};

struct CL_JOIN_ROOM {
    char room_name[32] = {0};
    char user_name[32] = {0};
    std::size_t user_id = 0;
};

struct SER_ROOM_JOINED {
    char room_name[32] = {0};
    char player_name[32] = {0};
};

struct CL_LEAVE_ROOM {
    char room_name[32] = {0};
    std::size_t user_id = 0;
};

struct SER_ROOM_LEAVED {
    char room_name[32] = {0};
    char player_name[32] = {0};
};

struct CL_READY {
    char room_name[32] = {0};
    std::size_t user_id = 0;
};

struct SER_READY {
    char room_name[32] = {0};
    char player_name[32] = {0};
};

struct CL_NOT_READY {
    char room_name[32] = {0};
    std::size_t user_id = 0;
};

struct SER_NOT_READY {
    char room_name[32] = {0};
    char player_name[32] = {0};
};

struct CL_ROOM_LIST {
    std::size_t user_id = 0;
};

struct SER_ROOM_LIST {
    char room_name[32] = {0};
};

struct SER_ROOM_CONTENT {
    char player_name[32] = {0};
    bool ready = false;
};

struct SER_ROOM_IN_GAME {
    char room_name[32] = {0};
};

struct SER_ROOM_READY {
    int port;
};
} // namespace rt::TCPData

namespace rt {

template <typename T>
struct TCPPacket {
    std::size_t magic = rt::TCP_MAGIC;
    std::size_t size = sizeof(*this);
    TCPCommand cmd;

    T data{};
};
} // namespace rt
