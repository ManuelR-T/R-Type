/*
** EPITECH PROJECT, 2024
** network
** File description:
** AsioServer
*/

#pragma once

#include "IServer.hpp"
#include <asio/io_context.hpp>

namespace server {
    class AsioServer: public IServer {
    public:
        AsioServer() = default;
        virtual ~AsioServer() = default;

    protected:
        virtual void asio_run() = 0;
        asio::io_context io_;
    };
}