/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <SFML/Graphics.hpp>
#include "RTypeConst.hpp"
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "TickRateManager.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/tag.hpp"
#include "components/velocity.hpp"
#include "core/Registry.hpp"
#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "udp/UDPServer.hpp"
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
    reg.registerComponent<ecs::component::Tag<size_t>>();
}

void rts::registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::TickRateManager &tickRateManager,
    ntw::UDPServer &udpServer,
    std::list<rt::UDPServerPacket> &datasToSend
)
{
    tickRateManager.addTickRate(rt::SEND_PACKETS_TICK_RATE);

    reg.addSystem([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.addSystem([&reg]() { ecs::systems::collision(reg); });
    reg.addSystem([&reg, &window]() { // ! for debug
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
    reg.addSystem([&reg]() { ecs::systems::missilesStop(reg); });
    reg.addSystem([&datasToSend, &udpServer, &tickRateManager, &dt]() {
        if (tickRateManager.needUpdate(rt::SEND_PACKETS_TICK_RATE, dt)) {
            while (!datasToSend.empty()) {
                udpServer.sendAll(reinterpret_cast<const char *>(&datasToSend.front()), sizeof(rt::UDPServerPacket));
                datasToSend.pop_front();
            }
        }
    });
}
