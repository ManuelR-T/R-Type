/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** main
*/

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <vector>
#include "RTypeServer.hpp"
#include "RoomManager.hpp"
#include "TCPResponseHandler.hpp"
#include "tcp/TCPServer.hpp"

int main()
{
    ntw::TCPServer tcpServer(8080);
    rts::RoomManager roomManager;
    rt::TCPResponseHandler responseHandler;

    rts::registerTcpResponse(roomManager, tcpServer, responseHandler);
    tcpServer.registerCommand([&responseHandler](tcp::socket &sock, char *data, std::size_t size) {
        responseHandler.handleResponse(data, size, {std::ref(sock)});
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
