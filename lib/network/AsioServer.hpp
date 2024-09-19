/*
** EPITECH PROJECT, 2024
** network
** File description:
** AsioServer
*/

#pragma once

#include "IServer.hpp"
#include <asio.hpp>

namespace server {

    /**
     *  @class AsioServer
     *  @brief AsioServer is a abstract class server that handle network with asio.
     *
     * NOTE: This class initialize the `io_context` for the asynchronous operations.
    */
    class AsioServer: public IServer {
    public:
        AsioServer() = default;
        ~AsioServer() override = default;

    protected:
        virtual void asio_run() = 0;
        asio::io_context io_;
    };
}