/*
** EPITECH PROJECT, 2024
** network
** File description:
** IClient
*/

#pragma once

#include <cstddef>

namespace ntw {
class IClient {
    public:
    IClient() = default;
    virtual ~IClient() = default;

    virtual void run() = 0;
    virtual void send(const char *data, std::size_t size) = 0;
};
} // namespace ntw
