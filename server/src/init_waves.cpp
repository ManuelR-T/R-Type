/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** init_waves
*/

#include "RTypeServer.hpp"
#include "ServerEntityFactory.hpp"
#include "WaveManager.hpp"

void rts::init_waves(ecs::WaveManager &waveManager, std::list<rt::UDPServerPacket> &datasToSend)
{
    auto waveId = waveManager.addNewWave();
    auto waveId2 = waveManager.addNewWave();

    for (int i = 1; i < 3; ++i) {
        waveManager.addNewMob(waveId, [i, &datasToSend](ecs::Registry &reg) {
            datasToSend.push_back(rt::UDPServerPacket(
                {.header = {.cmd = rt::UDPCommand::NEW_ENTITY},
                 .body = {.sharedEntityId = 0, .b = {.newEntityData = {0, {{100.f * i, 100.f * i}, {0}}}}}}
            ));
            return ecs::ServerEntityFactory::createServerEntityFromJSON(
                reg, "assets/static.json", 100.f * i, 100.f * i
            );
        });
        waveManager.addNewMob(waveId2, [i, &datasToSend](ecs::Registry &reg) {
            datasToSend.push_back(rt::UDPServerPacket(
                {.header = {.cmd = rt::UDPCommand::NEW_ENTITY},
                 .body = {.sharedEntityId = 0, .b = {.newEntityData = {0, {{100.f * i + 100, 100.f * i + 100}, {0}}}}}}
            ));
            return ecs::ServerEntityFactory::createServerEntityFromJSON(
                reg, "assets/static.json", 100.f * i, 100.f * i
            );
        });
    }
}
