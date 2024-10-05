/*
** EPITECH PROJECT, 2024
** network
** File description:
** IServer
*/

#pragma once

namespace ntw {

class IServer {
    public:
    IServer() = default;
    virtual ~IServer() = default;

    virtual void run() = 0;
};
} // namespace ntw
