/*
** EPITECH PROJECT, 2024
** network
** File description:
** IServer
*/

#pragma once

#include <cstddef>
#include <functional>

namespace server {
constexpr std::size_t BUFF_SIZE = 4096;

class IServer {
    public:
    IServer() = default;
    virtual ~IServer() = default;

    virtual void run() = 0;
};
} // namespace server
