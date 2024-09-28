/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include "rtype_server.hpp"

void rts::registerTcpResponse(
    rts::RoomManager &roomManager,
    server::TCPServer &tcpServer,
    ecs::ResponseHandler<rt::TcpCommand, rt::TcpPacket> &responseHandler
)
{
    responseHandler.registerHandler(rt::TcpCommand::CL_DISCONNECT_USER, [&tcpServer](const rt::TcpPacket &packet) {
        tcpServer.removeUser(packet.body.cl_new_user.user_id);
    });
    responseHandler.registerHandler(
        rt::TcpCommand::CL_CREATE_ROOM,
        [&roomManager, &tcpServer](const rt::TcpPacket &packet) {
            roomManager.createRoom(packet.body.cl_create_room.room_name, tcpServer);
        }
    );
    responseHandler.registerHandler(
        rt::TcpCommand::CL_DELETE_ROOM,
        [&roomManager, &tcpServer](const rt::TcpPacket &packet) {
            roomManager.deleteRoom(packet.body.cl_delete_room.room_name, tcpServer);
        }
    );
    responseHandler.registerHandler(
        rt::TcpCommand::CL_JOIN_ROOM,
        [&roomManager, &tcpServer](const rt::TcpPacket &packet) {
            roomManager.joinRoom(
                packet.body.cl_join_room.room_name,
                packet.body.cl_join_room.user_id,
                packet.body.cl_join_room.user_name,
                tcpServer
            );
        }
    );
    responseHandler.registerHandler(
        rt::TcpCommand::CL_LEAVE_ROOM,
        [&roomManager, &tcpServer](const rt::TcpPacket &packet) {
            roomManager.leaveRoom(packet.body.cl_leave_room.room_name, packet.body.cl_leave_room.user_id, tcpServer);
        }
    );
    responseHandler.registerHandler(rt::TcpCommand::CL_READY, [&roomManager, &tcpServer](const rt::TcpPacket &packet) {
        roomManager.playerReady(packet.body.cl_ready.room_name, packet.body.cl_ready.user_id, tcpServer);
    });
    responseHandler.registerHandler(
        rt::TcpCommand::CL_NOT_READY,
        [&roomManager, &tcpServer](const rt::TcpPacket &packet) {
            roomManager.playerNotReady(packet.body.cl_not_ready.room_name, packet.body.cl_not_ready.user_id, tcpServer);
        }
    );
    responseHandler.registerHandler(
        rt::TcpCommand::CL_ROOM_LIST,
        [&roomManager, &tcpServer](const rt::TcpPacket &packet) {
            roomManager.sendListRoom(packet.body.cl_room_list.user_id, tcpServer);
        }
    );
}
