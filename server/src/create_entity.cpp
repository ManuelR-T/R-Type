/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_entity
*/

#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <cstring>
#include "RTypeServer.hpp"

// ! It's a temporary file that will be delete when factory are setup

void rts::createMissile(ecs::Registry &reg, const rt::UDPClientPacket &msg)
{
    auto missile = reg.spawnSharedEntity(msg.body.sharedEntityId);

    const auto &pos = msg.body.b.newEntityData.moveData.pos;
    const auto &vel = msg.body.b.newEntityData.moveData.vel;

    reg.addComponent(missile, ecs::component::Position{pos.x + 10, pos.y + 20});
    reg.addComponent(missile, ecs::component::Velocity{vel.vx, vel.vy});
    reg.addComponent(missile, ecs::component::Hitbox{16.0, 16.0});

    ecs::component::Drawable playerDrawable;
    playerDrawable.shape.setSize(sf::Vector2f(20.f, 20.f));
    playerDrawable.shape.setFillColor(sf::Color::Yellow);
    reg.addComponent(missile, std::move(playerDrawable));

    reg.addComponent(missile, ecs::component::Missile{});
}
