/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** GameProtocol
*/

#ifndef GAMEPROTOCOL_HPP_
    #define GAMEPROTOCOL_HPP_

    #include <cstddef>
    #include <variant>

namespace ecs
{
    enum class ntw_action : std::size_t
    {
        NONE,
        NEW_PLAYER,
        NEW_ENTITY,
        MOD_ENTITY,
        DEL_ENTITY
    };

    struct protocol
    {
        ntw_action action = ntw_action::NONE;
        entity_t entity = 0;
        std::variant<
            std::monostate,
            ecs::component::position
        > data;
    };
}

#endif /* !GAMEPROTOCOL_HPP_ */
