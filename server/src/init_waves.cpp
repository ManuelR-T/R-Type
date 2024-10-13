/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** init_waves
*/

#include <cstdio>
#include <cstdlib>
#include <vector>
#include "RTypeConst.hpp"
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "ServerEntityFactory.hpp"
#include "WaveManager.hpp"
#include "ais/fireRandomMissileAi.hpp"
#include "ais/horizontalMoveAi.hpp"
#include "ais/waveAi.hpp"
#include "entity.hpp"
#include "components/ai_actor.hpp"
#include "shared_entity.hpp"

static entity_t spawnRobot(ecs::Registry &reg, std::list<std::vector<char>> &datasToSend, size_t maxMob, size_t i)
{
    float x = (float)rt::SCREEN_WIDTH * ((float)i / (float)(maxMob + 1));
    float y = (float)rt::SCREEN_HEIGHT * (4.f / 5.f);
    auto sharedId = ecs::generateSharedEntityId();
    datasToSend.push_back(rt::UDPPacket<rt::UDPBody::NEW_ENTITY_ROBOT_GROUND>(
                              {.cmd = rt::UDPCommand::NEW_ENTITY_ROBOT_GROUND,
                               .sharedEntityId = sharedId,
                               .body = {.moveData = {.pos = {x, y}, .vel = {-10, 0}}}}
    ).serialize());
    auto e = ecs::ServerEntityFactory::createServerEntityFromJSON(reg, "assets/robotGround.json", x, y, sharedId, -10);
    reg.getComponent<ecs::component::AiActor>(e)->act = [x, &datasToSend](ecs::Registry &reg, entity_t e) {
        rts::ais::horizontalMoveAi(reg, e, x - 100, x + 100);
        rts::ais::fireRandomMissileAi(reg, e, datasToSend, nullptr, {-1, 1}, {-1, 0});
    };
    return e;
}

static entity_t spawnBydosWave(ecs::Registry &reg, std::list<std::vector<char>> &datasToSend, size_t maxMob, size_t i)
{
    float x = (float)rt::SCREEN_WIDTH * (4.f / 5.f);
    float y = (float)rt::SCREEN_HEIGHT * ((float)i / (float)(maxMob + 1));
    auto sharedId = ecs::generateSharedEntityId();
    datasToSend.push_back(rt::UDPPacket<rt::UDPBody::NEW_ENTITY_BYDOS_WAVE>(
                              {.cmd = rt::UDPCommand::NEW_ENTITY_BYDOS_WAVE,
                               .sharedEntityId = sharedId,
                               .body = {.moveData = {.pos = {x, y}}}}
    ).serialize());
    auto e = ecs::ServerEntityFactory::createServerEntityFromJSON(reg, "assets/bydosWave.json", x, y, sharedId, -10);
    reg.getComponent<ecs::component::AiActor>(e)->act = [y, &datasToSend](ecs::Registry &reg, entity_t e) {
        rts::ais::waveMovement(reg, e, y);
        rts::ais::fireRandomMissileAi(reg, e, datasToSend);
    };
    return e;
}

// Create robot and put his ais
static void waveInit(ecs::WaveManager &waveManager, std::list<std::vector<char>> &datasToSend, size_t maxMob)
{
    auto waveId = waveManager.addNewWave();

    for (size_t i = 1; i < maxMob + 1; i++) {
        if (i % 2 == 0) {
            waveManager.addNewMob(waveId, [i, &datasToSend, maxMob](ecs::Registry &reg) {
                return spawnRobot(reg, datasToSend, maxMob, i);
            });
        } else {
            waveManager.addNewMob(waveId, [i, &datasToSend, maxMob](ecs::Registry &reg) {
                return spawnBydosWave(reg, datasToSend, maxMob, i);
            });
        }
    }
}

void rts::init_waves(ecs::WaveManager &waveManager, std::list<std::vector<char>> &datasToSend)
{
    auto waveId = waveManager.addNewWave();

    for (int i = 1; i < 3; ++i) {
        waveManager.addNewMob(waveId, [i, &datasToSend](ecs::Registry &reg) {
            datasToSend.push_back(rt::UDPPacket<rt::UDPBody::NEW_ENTITY_STATIC>(
                                      {.cmd = rt::UDPCommand::NEW_ENTITY_STATIC,
                                       .sharedEntityId = 0,
                                       .body = {.moveData = {.pos = {100.f * i, 100.f * i}}}}
            ).serialize());
            return ecs::ServerEntityFactory::createServerEntityFromJSON(
                reg, "assets/static.json", 100.f * i, 100.f * i
            );
        });
    }
    waveInit(waveManager, datasToSend, 5);
    waveInit(waveManager, datasToSend, 6);
}
