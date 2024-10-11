/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** init_waves
*/

#include <cstdio>
#include <cstdlib>
#include "RTypeConst.hpp"
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "ServerEntityFactory.hpp"
#include "WaveManager.hpp"
#include "ais/fireRandomMissileAi.hpp"
#include "ais/waveAi.hpp"
#include "entity.hpp"
#include "components/ai_actor.hpp"
#include "shared_entity.hpp"

static void bydosWaveInit(ecs::WaveManager &waveManager, std::list<rt::UDPServerPacket> &datasToSend)
{
    auto waveId = waveManager.addNewWave();
    constexpr size_t maxMob = 5;

    for (size_t i = 1; i < maxMob + 1; i++) {
        waveManager.addNewMob(waveId, [i, &datasToSend](ecs::Registry &reg) {
            float x = (float) rt::SCREEN_WIDTH * (4.f / 5.f);
            float y = (float) rt::SCREEN_HEIGHT * ((float) i / (float) (maxMob + 1));
            auto sharedId = ecs::generateSharedEntityId();
            datasToSend.push_back(rt::UDPServerPacket(
                {.header = {.cmd = rt::UDPCommand::NEW_ENTITY},
                 .body =
                     {.sharedEntityId = sharedId,
                      .b = {.newEntityData = {rt::EntityType::BYDOS_WAVE, {{x, y}, {0, 0}}}}}}
            ));
            auto e = ecs::ServerEntityFactory::createServerEntityFromJSON(
                reg, "assets/bydosWave.json", x, y, sharedId, -10
            );
            reg.getComponent<ecs::component::AiActor>(e)->act = [y, &datasToSend](ecs::Registry &reg, entity_t e) {
                rts::ais::waveMovement(reg, e, y);
                rts::ais::fireRandomMissileAi(reg, e, datasToSend);
            };
            return e;
        });
    }
}

void rts::init_waves(ecs::WaveManager &waveManager, std::list<rt::UDPServerPacket> &datasToSend)
{
    auto waveId = waveManager.addNewWave();

    for (int i = 1; i < 3; ++i) {
        waveManager.addNewMob(waveId, [i, &datasToSend](ecs::Registry &reg) {
            datasToSend.push_back(rt::UDPServerPacket(
                {.header = {.cmd = rt::UDPCommand::NEW_ENTITY},
                 .body =
                     {.sharedEntityId = 0,
                      .b = {.newEntityData = {rt::EntityType::STATIC, {{100.f * i + 100, 100.f * i + 100}, {0}}}}}}
            ));
            return ecs::ServerEntityFactory::createServerEntityFromJSON(
                reg, "assets/static.json", 100.f * i, 100.f * i
            );
        });
    }
    bydosWaveInit(waveManager, datasToSend);
}
