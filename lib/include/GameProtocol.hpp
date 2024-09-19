/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameProtocol
*/

#ifndef GAMEPROTOCOL_HPP_
    #define GAMEPROTOCOL_HPP_

    #include <cstddef>

namespace ecs
{
    enum class ntw_action : std::size_t
    {
        NONE,
        NEW_ENTITY,
        MOD_ENTITY,
        DEL_ENTITY
    };

    struct protocol
    {
        ntw_action action = ntw_action::NONE;
        std::size_t size = 0;
        char *data = nullptr;
    };
}

#endif /* !GAMEPROTOCOL_HPP_ */
