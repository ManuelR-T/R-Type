/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** response_handler
*/

#pragma once

#include <cstdio>
#include <cstring>
#include <functional>
#include <utility>
#include "TrackedException.hpp"
#include <unordered_map>

namespace ntw {

template <typename CommandType, typename PacketType>
class ResponseHandler {
    public:
    ResponseHandler(std::function<CommandType(const PacketType &)> cmdTypeGetter)
        : _cmdTypeGetter(std::move(cmdTypeGetter))
    {
    }

    void registerHandler(CommandType cmd, std::function<void(const PacketType &)> handler)
    {
        _handler[cmd] = std::move(handler);
    }

    void handleResponse(const char *data, std::size_t size)
    {
        PacketType msg{};
        CommandType cmdType;

        if (size != sizeof(msg)) {
            throw eng::TrackedException("Recv msg with bad size: " + std::to_string(size) + '.');
        }
        std::memcpy(&msg, data, sizeof(msg));
        cmdType = _cmdTypeGetter(msg);
        if (_handler.contains(cmdType)) {
            _handler[cmdType](msg);
        } else {
            throw eng::TrackedException(
                "Response without handler: " + std::to_string(static_cast<std::size_t>(cmdType)) + '.'
            );
        }
    }

    private:
    std::unordered_map<CommandType, std::function<void(const PacketType &)>> _handler;
    std::function<CommandType(const PacketType &)> _cmdTypeGetter;
};

} // namespace ntw
