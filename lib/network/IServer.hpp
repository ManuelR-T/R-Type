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
    constexpr size_t BUFF_SIZE = 4096;
    class IServer {
    public:
        IServer() = default;
        virtual ~IServer() = default;

        virtual void run() = 0;

        virtual void register_command(std::function<void (char *, std::size_t)> func) = 0;
    };
}
