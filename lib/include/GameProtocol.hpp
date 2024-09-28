/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameProtocol
*/

#pragma once

#include <cstddef>
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "core/shared_entity.hpp"

namespace rt {
enum class TcpCommand : std::size_t {
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

struct TcpPacket {
    TcpCommand cmd = TcpCommand::NONE;

    union {
        struct ClNewUser {
            std::size_t user_id = 0;
        } cl_new_user;

        struct ClDisconnectUser {
            std::size_t user_id = 0;
        } cl_disconnect_user;

        struct ClCreateRoom {
            char room_name[32] = {0};
        } cl_create_room;

        struct SerRoomCreated {
            char room_name[32] = {0};
        } ser_room_created;

        struct ClDeleteRoom {
            char room_name[32] = {0};
        } cl_delete_room;

        struct SerRoomDeleted {
            char room_name[32] = {0};
        } ser_room_deleted;

        struct ClJoinRoom {
            char room_name[32] = {0};
            char user_name[32] = {0};
            std::size_t user_id = 0;
        } cl_join_room;

        struct SerRoomJoined {
            char room_name[32] = {0};
            char player_name[32] = {0};
        } ser_room_joined;

        struct ClLeaveRoom {
            char room_name[32] = {0};
            std::size_t user_id = 0;
        } cl_leave_room;

        struct SerRoomLeaved {
            char room_name[32] = {0};
            char player_name[32] = {0};
        } ser_room_leaved;

        struct ClReady {
            char room_name[32] = {0};
            std::size_t user_id = 0;
        } cl_ready;

        struct SerReady {
            char room_name[32] = {0};
            char player_name[32] = {0};
        } ser_ready;

        struct ClNotReady {
            char room_name[32] = {0};
            std::size_t user_id = 0;
        } cl_not_ready;

        struct SerNotReady {
            char room_name[32] = {0};
            char player_name[32] = {0};
        } ser_not_ready;

        struct ClRoomList {
            std::size_t user_id = 0;
        } cl_room_list;

        struct SerRoomList {
            char room_name[32] = {0};
        } ser_room_list;

        struct SerRoomContent {
            char player_name[32] = {0};
            bool ready = false;
        } ser_room_content;

        struct SerRoomInGame {
            char room_name[32] = {0};
        } ser_room_in_game;

        struct SerRoomReady {
            int port;
        } ser_room_ready;
    } body = {};
};

} // namespace rt

namespace rt {
enum class UdpCommand : std::size_t {
    NONE,
    NEW_PLAYER,
    NEW_ENTITY,
    MOD_ENTITY,
    DEL_ENTITY
};

struct UdpPacket {
    UdpCommand cmd;

    shared_entity_t shared_entity_id;

    union {
        struct ShareMovement {
            ecs::component::Position pos;
            ecs::component::Velocity vel;
        } share_movement;
    } body = {};
};
} // namespace rt
