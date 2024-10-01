/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include "RTypeClient.hpp"
#include "RTypeConst.hpp"
#include "TickRateManager.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "core/SpriteManager.hpp"
#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "components/ai_actor.hpp"
#include "components/share_movement.hpp"
#include "systems/ai_act.hpp"
#include "systems/control_move.hpp"
#include "systems/control_special.hpp"
#include "systems/missiles_stop.hpp"
#include "systems/share_movement.hpp"
#include "systems/sprite_system.hpp"

void rtc::registerComponents(ecs::Registry &reg)
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

void rtc::registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::UDPClient &udpClient,
    ecs::InputManager &input,
    ntw::TickRateManager &tickRateManager,
    ecs::SpriteManager &spriteManager
)
{
    tickRateManager.addTickRate(rt::MOVEMENT_TICK_RATE);
    tickRateManager.addTickRate(10);

    reg.addSystem([&reg, &input]() { ecs::systems::controlMove(reg, input); });
    reg.addSystem([&reg, &input, &udpClient, &spriteManager]() {
        ecs::systems::controlSpecial(reg, input, udpClient, spriteManager);
    });
    reg.addSystem([&reg, &dt, &tickRateManager]() {
        if (tickRateManager.needUpdate(10, dt)) {
            ecs::systems::aiAct(reg);
        }
    });
    reg.addSystem([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.addSystem([&reg]() { ecs::systems::collision(reg); });
    reg.addSystem([&reg, &udpClient, &tickRateManager, &dt]() {
        if (tickRateManager.needUpdate(rt::MOVEMENT_TICK_RATE, dt)) {
            ecs::systems::shareMovement(reg, udpClient);
        }
    });
    reg.addSystem([&reg]() { ecs::systems::missilesStop(reg); });
    reg.addSystem([&reg, &dt, &spriteManager]() { ecs::systems::spriteSystem(reg, dt, spriteManager); });
    reg.addSystem([&reg, &window]() {
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
}
