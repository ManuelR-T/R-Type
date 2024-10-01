/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <SFML/Graphics.hpp>
#include "RTypeServer.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "core/Registry.hpp"
#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "components/ai_actor.hpp"
#include "components/share_movement.hpp"
#include "components/shared_entity.hpp"
#include "systems/missiles_stop.hpp"

void rts::registerComponents(ecs::Registry &reg)
{
    reg.registerComponent<ecs::component::Position>();
    reg.registerComponent<ecs::component::Velocity>();
    reg.registerComponent<ecs::component::Drawable>();
    reg.registerComponent<ecs::component::Sprite>();
    reg.registerComponent<ecs::component::Animation>();
    reg.registerComponent<ecs::component::Controllable>();
    reg.registerComponent<ecs::component::Hitbox>();
    reg.registerComponent<ecs::component::ShareMovement>();
    reg.registerComponent<ecs::component::SharedEntity>();
    reg.registerComponent<ecs::component::Missile>();
    reg.registerComponent<ecs::component::AiActor>();
}

void rts::registerSystems(ecs::Registry &reg, sf::RenderWindow &window, float &dt)
{
    reg.addSystem([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.addSystem([&reg]() { ecs::systems::collision(reg); });
    reg.addSystem([&reg, &window]() { // ! for debug
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
    reg.addSystem([&reg]() { ecs::systems::missilesStop(reg); });
}
