/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <list>
#include "ClientTickRate.hpp"
#include "RTypeClient.hpp"
#include "SpriteManager.hpp"
#include "TickRateManager.hpp"
#include "components/animation.hpp"
#include "components/beam.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/health.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/parallax.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/score.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "imgui.h"
#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/parallax.hpp"
#include "systems/position.hpp"
#include "components/music_component.hpp"
#include "components/share_movement.hpp"
#include "components/sound_emitter.hpp"
#include "imgui-SFML.h"
#include "systems/control_move.hpp"
#include "systems/control_special.hpp"
#include "systems/draw_player_beam_bar.hpp"
#include "systems/draw_player_health_bar.hpp"
#include "systems/draw_score.hpp"
#include "systems/draw_team_data.hpp"
#include "systems/health_check.hpp"
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
    reg.registerComponent<ecs::component::Parallax>();
    reg.registerComponent<ecs::component::Health>();
    reg.registerComponent<ecs::component::SoundEmitter>();
    reg.registerComponent<ecs::component::MusicComponent>();
    reg.registerComponent<ecs::component::Beam>();
    reg.registerComponent<ecs::component::Score>();
    reg.registerComponent<ecs::component::Player>();
}

void rtc::registerSystems(
    ecs::Registry &reg,
    sf::RenderWindow &window,
    float &dt,
    ntw::UDPClient &udpClient,
    ecs::InputManager &input,
    ntw::TickRateManager<rtc::TickRate> &tickRateManager,
    ecs::SpriteManager &spriteManager,
    std::list<std::function<void(ecs::Registry &reg)>> &_networkCallbacks
)
{
    tickRateManager.addTickRate(rtc::TickRate::MOVEMENT, rtc::CLIENT_TICKRATE.at(rtc::TickRate::MOVEMENT));
    tickRateManager.addTickRate(
        rtc::TickRate::CALL_NETWORK_CALLBACKS, rtc::CLIENT_TICKRATE.at(rtc::TickRate::CALL_NETWORK_CALLBACKS)
    );

    reg.addSystem([&reg, &input]() { ecs::systems::controlMove(reg, input); });
    reg.addSystem([&reg, &input, &udpClient, &spriteManager]() {
        ecs::systems::controlSpecial(reg, input, udpClient, spriteManager);
    });
    reg.addSystem([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.addSystem([&reg]() { ecs::systems::collision(reg); });
    reg.addSystem([&reg, &udpClient, &tickRateManager, &dt]() {
        if (tickRateManager.needUpdate(rtc::TickRate::MOVEMENT, dt)) {
            ecs::systems::shareMovement(reg, udpClient);
        }
    });
    reg.addSystem([&reg]() { ecs::systems::missilesStop(reg); });
    reg.addSystem([&reg]() { ecs::systems::healthCheck(reg); });
    reg.addSystem([&reg]() { ecs::systems::parallax(reg); });
    reg.addSystem([&reg, &dt, &spriteManager]() { ecs::systems::spriteSystem(reg, dt, spriteManager); });
    reg.addSystem([&reg, &window, &dt]() { ecs::systems::draw(reg, window); });
    reg.addSystem([&_networkCallbacks, &tickRateManager, &dt, &reg]() {
        if (tickRateManager.needUpdate(rtc::TickRate::CALL_NETWORK_CALLBACKS, dt)) {
            while (!_networkCallbacks.empty()) {
                _networkCallbacks.front()(reg);
                _networkCallbacks.pop_front();
            }
        }
    });
    reg.addSystem([&reg, &window]() { ecs::systems::drawPlayerBeamBar(reg, window.getSize()); });
    reg.addSystem([&reg, &window]() { ecs::systems::drawPlayerHealthBar(reg, window.getSize()); });
    reg.addSystem([&reg, &window]() { ecs::systems::drawScore(reg, window, window.getSize()); });
    reg.addSystem([&reg, &window]() { ecs::systems::drawTeamData(reg, window.getSize()); });
}
