/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** main
*/

#include "RTypeServer.hpp"
#include "RTypeTCPProtol.hpp"
#include "ResponseHandler.hpp"
#include "RoomManager.hpp"

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <cstring>
#include "tcp/TCPServer.hpp"

#include <iostream>

int main()
{
    ntw::TCPServer tcpServer(8080, sizeof(rt::TCPPacket));
    rts::RoomManager roomManager;
    ntw::ResponseHandler<rt::TCPCommand, rt::TCPPacket> responseHandler([](const rt::TCPPacket &packet) {
        return packet.cmd;
    });

    rts::registerTcpResponse(roomManager, tcpServer, responseHandler);
    tcpServer.registerCommand([&responseHandler, &tcpServer](tcp::socket &sock, char *data, std::size_t size) {
        rt::TCPCommand newUserCmd = rt::TCPCommand::CL_NEW_USER;

        if (std::memcmp(data, &newUserCmd, sizeof(rt::TCPCommand)) == 0) {
            rt::TCPPacket packet{};
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
