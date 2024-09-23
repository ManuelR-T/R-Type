/*
** EPITECH PROJECT, 2024
** network
** File description:
** IClient
*/

#pragma once

#include <cstddef>

namespace client {
constexpr std::size_t BUFF_SIZE = 4096;

class IClient {
    public:
    IClient() = default;
    virtual ~IClient() = default;

    virtual void run() = 0;
    virtual void send(const char *data, std::size_t size) = 0;
};
} // namespace client
