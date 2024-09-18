/*
** EPITECH PROJECT, 2024
** network
** File description:
** AsioClient
*/

#pragma once

#include "IClient.hpp"
#include <asio/io_context.hpp>

namespace client {
    class AsioClient: public IClient {
    public:
        AsioClient() = default;
        virtual ~AsioClient() = default;

    protected:
        virtual void asio_run() = 0;
        asio::io_context io_;
    };
}
