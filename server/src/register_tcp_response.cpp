/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include "rtype_server.hpp"

void rts::register_tcp_response(
    rts::room_manager &room_manager,
    server::TCPServer &tcpServer,
    ecs::response_handler<rt::tcp_command, rt::tcp_packet> &response_handler
)
{
    response_handler.register_handler(rt::tcp_command::CL_DISCONNECT_USER, [&tcpServer](const rt::tcp_packet &packet) {
        tcpServer.remove_user(packet.body.cl_new_user.user_id);
    });
    response_handler.register_handler(
        rt::tcp_command::CL_CREATE_ROOM,
        [&room_manager, &tcpServer](const rt::tcp_packet &packet) {
            room_manager.create_room(packet.body.cl_create_room.room_name, tcpServer);
        }
    );
    response_handler.register_handler(
        rt::tcp_command::CL_DELETE_ROOM,
        [&room_manager, &tcpServer](const rt::tcp_packet &packet) {
            room_manager.delete_room(packet.body.cl_delete_room.room_name, tcpServer);
        }
    );
    response_handler.register_handler(
        rt::tcp_command::CL_JOIN_ROOM,
        [&room_manager, &tcpServer](const rt::tcp_packet &packet) {
            room_manager.join_room(
                packet.body.cl_join_room.room_name,
                packet.body.cl_join_room.user_id,
                packet.body.cl_join_room.user_name,
                tcpServer
            );
        }
    );
    response_handler.register_handler(
        rt::tcp_command::CL_LEAVE_ROOM,
        [&room_manager, &tcpServer](const rt::tcp_packet &packet) {
            room_manager.leave_room(packet.body.cl_leave_room.room_name, packet.body.cl_leave_room.user_id, tcpServer);
        }
    );
    response_handler.register_handler(
        rt::tcp_command::CL_READY,
        [&room_manager, &tcpServer](const rt::tcp_packet &packet) {
            room_manager.player_ready(packet.body.cl_ready.room_name, packet.body.cl_ready.user_id, tcpServer);
        }
    );
    response_handler.register_handler(
        rt::tcp_command::CL_NOT_READY,
        [&room_manager, &tcpServer](const rt::tcp_packet &packet) {
            room_manager.player_not_ready(
                packet.body.cl_not_ready.room_name, packet.body.cl_not_ready.user_id, tcpServer
            );
        }
    );
    response_handler.register_handler(
        rt::tcp_command::CL_ROOM_LIST,
        [&room_manager, &tcpServer](const rt::tcp_packet &packet) {
            room_manager.send_list_room(packet.body.cl_room_list.user_id, tcpServer);
        }
    );
}
