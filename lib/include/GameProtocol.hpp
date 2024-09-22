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

    namespace ntw
    {
        struct movement
        {
            ecs::component::position pos;
            ecs::component::velocity vel;
        };
    }

    struct protocol
    {
        ntw_action action;
        shared_entity_t shared_entity_id;
        std::variant<
            std::monostate,
            ntw::movement> data;
    };
}

#endif /* !GAMEPROTOCOL_HPP_ */
