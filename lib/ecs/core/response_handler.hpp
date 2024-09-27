/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** response_handler
*/

#pragma once

#include <cstring>
#include <functional>
#include <utility>
#include "GameProtocol.hpp"
#include "tracked_exception.hpp"
#include <unordered_map>

namespace ecs {
template <typename CommandType, typename PacketType>
class response_handler {
    public:
    response_handler(std::function<CommandType (const PacketType &)> cmd_type_getter)
        : _cmd_type_getter(cmd_type_getter)
    {
    }

    void register_handler(CommandType cmd, std::function<void(const PacketType &)> handler)
    {
        _handler[cmd] = std::move(handler);
    }

    void handle_response(const char *data, std::size_t size)
    {
        PacketType msg{};
        CommandType cmd_type;

        std::memcpy(&msg, data, sizeof(msg));
        cmd_type = _cmd_type_getter(msg);
        if (_handler.contains(cmd_type)) {
            _handler[cmd_type](msg);
        } else {
            throw ecs::tracked_exception(
                "Response without handler: " + std::to_string(static_cast<std::size_t>(cmd_type)) + "."
            );
        }
    }

    private:
    std::unordered_map<CommandType, std::function<void(const PacketType &)>> _handler;
    std::function<CommandType (const PacketType &)> _cmd_type_getter;
};
} // namespace ecs
