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

/**
 *  @class AsioClient
 *  @brief AsioClient is a abstract class client that handle network with asio.
 *
 *  NOTE: This class initialize the `io_context` for the asynchronous operations.
 */
class AsioClient : public IClient {
    public:
    AsioClient() = default;
    ~AsioClient() override = default;

    protected:
    virtual void _asioRun() = 0;
    asio::io_context _io;
};
} // namespace client
