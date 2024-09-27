/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameProtocol
*/

#pragma once

#include <cstddef>
#include <variant>
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "core/shared_entity.hpp"

namespace rt {
enum class tcp_command : std::size_t {
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

struct tcp_packet {
    tcp_command cmd = tcp_command::NONE;

    union {
        struct cl_new_user {
            std::size_t user_id = 0;
        } cl_new_user;

        struct cl_disconnect_user {
            std::size_t user_id = 0;
        } cl_disconnect_user;

        struct cl_create_room {
            char room_name[32] = {0};
            std::size_t user_id = 0;
        } cl_create_room;

        struct ser_room_created {
            char room_name[32] = {0};
        } ser_room_created;

        struct cl_delete_room {
            char room_name[32] = {0};
            std::size_t user_id = 0;
        } cl_delete_room;

        struct ser_room_deleted {
            char room_name[32] = {0};
        } ser_room_deleted;

        struct cl_join_room {
            char room_name[32] = {0};
            char user_name[32] = {0};
            std::size_t user_id = 0;
        } cl_join_room;

        struct ser_room_joined {
            char room_name[32] = {0};
            char player_name[32] = {0};
        } ser_room_joined;

        struct cl_leave_room {
            char room_name[32] = {0};
            std::size_t user_id = 0;
        } cl_leave_room;

        struct ser_room_leaved {
            char room_name[32] = {0};
            char player_name[32] = {0};
        } ser_room_leaved;

        struct cl_ready {
            char room_name[32] = {0};
            std::size_t user_id = 0;
        } cl_ready;

        struct ser_ready {
            char room_name[32] = {0};
            char player_name[32] = {0};
        } ser_ready;

        struct cl_not_ready {
            char room_name[32] = {0};
            std::size_t user_id = 0;
        } cl_not_ready;

        struct ser_not_ready {
            char room_name[32] = {0};
            char player_name[32] = {0};
        } ser_not_ready;

        struct cl_room_list {
            std::size_t user_id = 0;
        } cl_room_list;

        struct ser_room_list {
            char room_name[32] = {0};
        } ser_room_list;

        struct ser_room_content {
            char player_name[32] = {0};
            bool ready = false;
        } ser_room_content;

        struct ser_room_in_game {
            char room_name[32] = {0};
        } ser_room_in_game;

        struct ser_room_ready {
            int port;
        } ser_room_ready;
    } body;
};

} // namespace rt

namespace rt {
enum class udp_command : std::size_t {
    NONE,
    NEW_PLAYER,
    NEW_ENTITY,
    MOD_ENTITY,
    DEL_ENTITY
};

struct udp_packet {
    udp_command cmd;

    shared_entity_t shared_entity_id;

    union {
        struct share_movement {
            ecs::component::position pos;
            ecs::component::velocity vel;
        } share_movement;
    } body;
};
} // namespace rt
