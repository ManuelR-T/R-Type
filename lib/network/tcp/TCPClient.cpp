/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** TCPClient
*/

#include "TCPClient.hpp"
#include <asio/ip/address_v4.hpp>


server::TCPClient::TCPClient(const std::string &host, int port):
    socket_(io_),
    host_(host),
    port_(port)
{
    socket_.connect(
        tcp::endpoint(asio::ip::address::from_string(host.c_str()),
        port_
    ));
}
