/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** response_handler
*/

#pragma once

#include <cstring>
#include <functional>
#include "GameProtocol.hpp"
#include "tracked_exception.hpp"
#include <unordered_map>

namespace ecs {
class response_handler {
    public:
    void register_handler(ecs::ntw_action action, std::function<void(ecs::protocol &)> handler)
    {
        _handler[action] = handler;
    }

    void handle_response(char *data, std::size_t size)
    {
        ecs::protocol msg{};

        std::memcpy(&msg, data, sizeof(msg));
        if (_handler.contains(msg.action)) {
            _handler[msg.action](msg);
        } else {
            throw ecs::tracked_exception(
                "Response without handler: " + std::to_string(static_cast<std::size_t>(msg.action)) + "."
            );
        }
    }

    private:
    std::unordered_map<ecs::ntw_action, std::function<void(ecs::protocol &)>> _handler;
};
} // namespace ecs
