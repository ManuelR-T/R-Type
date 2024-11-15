/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <SFML/Graphics.hpp>
#include <cstdio>
#include <functional>
#include <memory>
#include <vector>
#include "components/death_split.hpp"
#include "components/game_tag.hpp"
#include "systems/check_game_ending.hpp"
#include "systems/check_out_of_range.hpp"
#include <imgui-SFML.h>

#include "RTypeServer.hpp"
#include "Registry.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "ServerTickRate.hpp"
#include "TickRateManager.hpp"
#include "components/dobkeratops.hpp"
#include "gameCallbacks/collideEffect.hpp"
#include "gameCallbacks/endGame.hpp"
#include "udp/UDPServer.hpp"

#include "components/animation.hpp"
#include "components/beam.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/gravity.hpp"
#include "components/health.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/parallax.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/score.hpp"
#include "components/sprite.hpp"
#include "components/tag.hpp"
#include "components/velocity.hpp"
#include "components/ai_actor.hpp"
#include "components/health_xp.hpp"
#include "components/is_a_boss.hpp"
#include "components/server_share_movement.hpp"
#include "components/shared_entity.hpp"

#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/parallax.hpp"
#include "systems/position.hpp"
#include "systems/ai_act.hpp"
#include "systems/health_mob_check.hpp"
#include "systems/health_shared_check.hpp"
#include "systems/missiles_stop.hpp"
#include "systems/server_share_movement.hpp"

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
    reg.registerComponent<ecs::component::ServerShareMovement>();
    reg.registerComponent<ecs::component::SharedEntity>();
    reg.registerComponent<ecs::component::Missile>();
    reg.registerComponent<ecs::component::AiActor>();
    reg.registerComponent<ecs::component::Tag<ecs::component::EntityTag>>();
    reg.registerComponent<ecs::component::Health>();
    reg.registerComponent<ecs::component::AiActor>();
    reg.registerComponent<ecs::component::Health>();
    reg.registerComponent<ecs::component::Beam>();
    reg.registerComponent<ecs::component::Score>();
    reg.registerComponent<ecs::component::Player>();
    reg.registerComponent<ecs::component::DobkeratopsState>();
    reg.registerComponent<ecs::component::DeathSplit>();
    reg.registerComponent<ecs::component::IsABoss>();
    reg.registerComponent<ecs::component::Gravity>();
    reg.registerComponent<ecs::component::HealthXP>();
}

void rts::registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::TickRateManager<rts::TickRate> &tickRateManager,
    ntw::UDPServer &udpServer,
    std::list<std::vector<char>> &datasToSend,
    ntw::TimeoutHandler &timeoutHandler,
    eng::SafeList<std::function<void(ecs::Registry &reg)>> &networkCallbacks,
    ecs::WaveManager &waveManager,
    bool debugMode,
    size_t &nbPlayers
)
{
    tickRateManager.addTickRate(rts::TickRate::SEND_PACKETS, rts::SERVER_TICKRATE.at(rts::TickRate::SEND_PACKETS));
    tickRateManager.addTickRate(rts::TickRate::AI_ACTING, rts::SERVER_TICKRATE.at(rts::TickRate::AI_ACTING));
    tickRateManager.addTickRate(
        rts::TickRate::ENTITY_MOVEMENT, rts::SERVER_TICKRATE.at(rts::TickRate::ENTITY_MOVEMENT)
    );

    reg.addSystem([&networkCallbacks]() { networkCallbacks.consumeList(); });
    reg.addSystem([&reg, &dt, &tickRateManager]() {
        if (tickRateManager.needUpdate(rts::TickRate::AI_ACTING, dt)) {
            ecs::systems::aiAct(reg);
        }
    });
    reg.addSystem([&reg]() { ecs::systems::parallax(reg); });
    reg.addSystem([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.addSystem([&reg, &datasToSend, &udpServer, &timeoutHandler]() {
        ecs::systems::collision(
            reg,
            datasToSend,
            [&udpServer, &timeoutHandler](
                ecs::Registry &reg, entity_t entityA, entity_t entityB, std::list<std::vector<char>> &datasToSend
            ) { collideEffect(reg, entityA, entityB, datasToSend, udpServer, timeoutHandler); }
        );
    });
    reg.addSystem([&reg, &waveManager, &datasToSend, &udpServer, &timeoutHandler]() {
        if (waveManager.hasEntity()) {
            ecs::systems::checkOutOfRange(reg, waveManager, datasToSend, udpServer, timeoutHandler);
        }
        ecs::systems::healthMobCheck(reg, waveManager);
        ecs::systems::healthSharedCheck(reg, datasToSend, udpServer, timeoutHandler);
    });
    reg.addSystem([&reg, &waveManager]() {
        if (!waveManager.hasEntity() && !waveManager.isEnd()) {
            waveManager.spawnNextWave(reg);
        }
    });
    reg.addSystem([&reg, &datasToSend, &udpServer, &timeoutHandler]() {
        ecs::systems::missilesStop(reg, datasToSend, udpServer, timeoutHandler);
    });
    if (debugMode) {
        reg.addSystem([&reg, &window]() {
            window.clear();
            ecs::systems::draw(reg, window, std::shared_ptr<sf::Shader>());
            window.display();
        });
    }
    reg.addSystem([&datasToSend, &tickRateManager, &dt, &reg]() {
        if (tickRateManager.needUpdate(rts::TickRate::ENTITY_MOVEMENT, dt)) {
            ecs::systems::serverShareMovement(reg, datasToSend);
        }
    });
    reg.addSystem([&nbPlayers, &waveManager, &reg, &datasToSend]() {
        ecs::systems::checkGameEnding(
            reg,
            [&nbPlayers](ecs::Registry &) { return nbPlayers == 0; },
            [&datasToSend](ecs::Registry &) { endGame(datasToSend); }
        );
        ecs::systems::checkGameEnding(
            reg,
            [&waveManager](ecs::Registry &) { return !waveManager.hasEntity() && waveManager.isEnd(); },
            [&datasToSend](ecs::Registry &) { endGame(datasToSend, true); }
        );
    });
    reg.addSystem([&datasToSend, &udpServer, &tickRateManager, &dt]() {
        if (tickRateManager.needUpdate(rts::TickRate::SEND_PACKETS, dt)) {
            while (!datasToSend.empty()) {
                udpServer.sendAll(
                    reinterpret_cast<const char *>(datasToSend.front().data()), datasToSend.front().size()
                );
                datasToSend.pop_front();
            }
        }
    });
}
