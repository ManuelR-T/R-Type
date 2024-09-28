/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** main
*/

#include "GameProtocol.hpp"
#include "core/response_handler.hpp"
#include "room_manager.hpp"
#include "rtype_server.hpp"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <cstring>
#include "TCPServer.hpp"

#include <iostream>

int main()
{
    server::TCPServer tcpServer(8080, sizeof(rt::TcpPacket));
    rts::RoomManager roomManager;
    ecs::ResponseHandler<rt::TcpCommand, rt::TcpPacket> responseHandler([](const rt::TcpPacket &packet) {
        return packet.cmd;
    });

    rts::registerTcpResponse(roomManager, tcpServer, responseHandler);
    tcpServer.registerCommand([&responseHandler, &tcpServer](tcp::socket &sock, char *data, std::size_t size) {
        rt::TcpCommand newUserCmd = rt::TcpCommand::CL_NEW_USER;

        if (std::memcmp(data, &newUserCmd, sizeof(rt::TcpCommand)) == 0) {
            rt::TcpPacket packet{};
            std::memcpy(&packet, data, sizeof(packet));
            tcpServer.addUser(sock, packet.body.cl_new_user.user_id);
        } else {
            responseHandler.handleResponse(data, size);
        }
    });

    tcpServer.run();

    std::string str;
    while (std::getline(std::cin, str)) {
        if (str == "quit" || str == "exit") {
            break;
        }
    }
    return 0;
}
