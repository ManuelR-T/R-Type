/*
** EPITECH PROJECT, 2024
** network
** File description:
** IServer
*/

#pragma once

#include <cstddef>
#include <functional>

#define BUFF_SIZE 256

namespace server {
    class IServer {
    public:
        IServer() = default;
        virtual ~IServer() = default;

        virtual void run() = 0;

        virtual void register_command(
            char const *name,
            std::function<void (char *, std::size_t)> func
        ) = 0;
    };
}
