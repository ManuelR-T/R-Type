/*
** EPITECH PROJECT, 2024
** network
** File description:
** AsioClient
*/

#pragma once

#include "IClient.hpp"
#include <asio.hpp>

namespace client {

    /**
     *  @class AsioClient
     *  @brief AsioClient is a abstract class client that handle network with asio.
     *
     *  NOTE: This class initialize the `io_context` for the asynchronous operations.
    */
    class AsioClient: public IClient {
    public:
        AsioClient() = default;
        ~AsioClient() override = default;

    protected:
        virtual void asio_run() = 0;
        asio::io_context io_;
    };
}
