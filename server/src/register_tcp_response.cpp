/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <any>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>
#include "RTypeServer.hpp"
#include "RTypeTCPProtol.hpp"
#include "TCPResponseHandler.hpp"

void rts::registerTcpResponse(
    rts::RoomManager &roomManager,
    ntw::TCPServer &tcpServer,
    rt::TCPResponseHandler &responseHandler
)
{
    tcpServer.registerDisconnectionHandler([&roomManager, &tcpServer](std::size_t user_id) {
        roomManager.playerDisconnected(user_id, tcpServer);
    });
    responseHandler.registerHandler<rt::TCPData::CL_NEW_USER>(
        rt::TCPCommand::CL_NEW_USER,
        [&tcpServer](const rt::TCPPacket<rt::TCPData::CL_NEW_USER> &packet, const std::vector<std::any> &arg) {
            tcpServer.addUser(std::any_cast<std::reference_wrapper<tcp::socket>>(arg.at(0)).get(), packet.data.user_id);
        }
    );
    responseHandler.registerHandler<rt::TCPData::CL_CREATE_ROOM>(
        rt::TCPCommand::CL_CREATE_ROOM,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPData::CL_CREATE_ROOM> &packet) {
            roomManager.createRoom(packet.data.room_name, packet.data.stage, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPData::CL_DELETE_ROOM>(
        rt::TCPCommand::CL_DELETE_ROOM,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPData::CL_DELETE_ROOM> &packet) {
            roomManager.deleteRoom(packet.data.room_name, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPData::CL_JOIN_ROOM>(
        rt::TCPCommand::CL_JOIN_ROOM,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPData::CL_JOIN_ROOM> &packet) {
            roomManager.joinRoom(packet.data.room_name, packet.data.user_id, packet.data.user_name, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPData::CL_LEAVE_ROOM>(
        rt::TCPCommand::CL_LEAVE_ROOM,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPData::CL_LEAVE_ROOM> &packet) {
            roomManager.leaveRoom(packet.data.room_name, packet.data.user_id, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPData::CL_READY>(
        rt::TCPCommand::CL_READY,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPData::CL_READY> &packet) {
            roomManager.playerReady(packet.data.room_name, packet.data.user_id, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPData::CL_NOT_READY>(
        rt::TCPCommand::CL_NOT_READY,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPData::CL_NOT_READY> &packet) {
            roomManager.playerNotReady(packet.data.room_name, packet.data.user_id, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPData::CL_ROOM_LIST>(
        rt::TCPCommand::CL_ROOM_LIST,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPData::CL_ROOM_LIST> &packet) {
            roomManager.sendListRoom(packet.data.user_id, tcpServer);
        }
    );
    responseHandler.registerHandler<rt::TCPData::CL_UDP_CONNECTION_READY>(
        rt::TCPCommand::CL_UDP_CONNECTION_READY,
        [&roomManager, &tcpServer](const rt::TCPPacket<rt::TCPData::CL_UDP_CONNECTION_READY> &packet) {
            roomManager.udpPlayerReady(packet.data.room_name, packet.data.user_id, tcpServer);
        }
    );
}
