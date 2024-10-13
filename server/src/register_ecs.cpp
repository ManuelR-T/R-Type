/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <SFML/Graphics.hpp>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <vector>
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "ServerTickRate.hpp"
#include "TickRateManager.hpp"
#include "Zipper.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/health.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/parallax.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/tag.hpp"
#include "components/velocity.hpp"
#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "udp/UDPServer.hpp"
#include "components/ai_actor.hpp"
#include "components/share_movement.hpp"
#include "components/shared_entity.hpp"
#include "systems/ai_act.hpp"
#include "systems/health_check.hpp"
#include "systems/health_mob_check.hpp"
#include "systems/missiles_stop.hpp"

static void share_server_movements(ecs::Registry &reg, std::list<std::vector<char>> &datasToSend)
{
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &velocities = reg.getComponents<ecs::component::Velocity>();
    auto &sharedEntity = reg.getComponents<ecs::component::SharedEntity>();

    ecs::Zipper<ecs::component::Position, ecs::component::Velocity, ecs::component::SharedEntity> zip(
        positions, velocities, sharedEntity
    );

    for (auto [pos, vel, shared_entity] : zip) {
        datasToSend.push_back(rt::UDPPacket<rt::UDPBody::MOVE_ENTITY>({.cmd = rt::UDPCommand::MOVE_ENTITY,
                                                                       .sharedEntityId = shared_entity.sharedEntityId,
                                                                       .body = {.pos = pos, .vel = vel}}
        ).serialize());
    }
}

void rts::registerComponents(ecs::Registry &reg)
{
    reg.registerComponent<ecs::component::Position>();
    reg.registerComponent<ecs::component::Velocity>();
    reg.registerComponent<ecs::component::Drawable>();
    reg.registerComponent<ecs::component::Sprite>();
    reg.registerComponent<ecs::component::Animation>();
    reg.registerComponent<ecs::component::Parallax>();
    reg.registerComponent<ecs::component::Controllable>();
    reg.registerComponent<ecs::component::Hitbox>();
    reg.registerComponent<ecs::component::ShareMovement>();
    reg.registerComponent<ecs::component::SharedEntity>();
    reg.registerComponent<ecs::component::Missile>();
    reg.registerComponent<ecs::component::AiActor>();
    reg.registerComponent<ecs::component::Tag<size_t>>();
    reg.registerComponent<ecs::component::Health>();
    reg.registerComponent<ecs::component::AiActor>();
    reg.registerComponent<ecs::component::Player>();
}

void rts::registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::TickRateManager<rts::TickRate> &tickRateManager,
    ntw::UDPServer &udpServer,
    std::list<std::vector<char>> &datasToSend,
    std::list<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    ecs::WaveManager &waveManager
)
{
    tickRateManager.addTickRate(rts::TickRate::SEND_PACKETS, rts::SERVER_TICKRATE.at(rts::TickRate::SEND_PACKETS));
    tickRateManager.addTickRate(rts::TickRate::AI_ACTING, rts::SERVER_TICKRATE.at(rts::TickRate::AI_ACTING));

    reg.addSystem([&networkCallbacks, &reg]() {
        while (!networkCallbacks.empty()) {
            networkCallbacks.front()(reg);
            networkCallbacks.pop_front();
        }
    });
    reg.addSystem([&reg, &dt, &tickRateManager]() {
        if (tickRateManager.needUpdate(rts::TickRate::AI_ACTING, dt)) {
            ecs::systems::aiAct(reg);
        }
    });
    reg.addSystem([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.addSystem([&reg]() { ecs::systems::collision(reg); });
    reg.addSystem([&reg, &waveManager]() {
        ecs::systems::healthMobCheck(reg, waveManager);
        ecs::systems::healthCheck(reg);
    });
    reg.addSystem([&reg, &waveManager]() {
        if (!waveManager.hasEntity() && !waveManager.isEnd()) {
            waveManager.spawnNextWave(reg);
        }
    });
    reg.addSystem([&reg]() { ecs::systems::missilesStop(reg); });
    reg.addSystem([&reg, &window]() { // ! for debug
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
    reg.addSystem([&datasToSend, &udpServer, &tickRateManager, &dt, &reg]() {
        if (tickRateManager.needUpdate(rts::TickRate::SEND_PACKETS, dt)) {
            share_server_movements(reg, datasToSend);
            while (!datasToSend.empty()) {
                udpServer.sendAll(
                    reinterpret_cast<const char *>(datasToSend.front().data()), datasToSend.front().size()
                );
                datasToSend.pop_front();
            }
        }
    });
}
