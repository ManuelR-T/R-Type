/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_player_health_bar
*/

#include "draw_player_health_bar.hpp"
#include <string>
#include "Zipper.hpp"
#include "components/health.hpp"
#include "components/player.hpp"
#include "imgui.h"
#include "components/self_player.hpp"

static void drawBar(int percentageBar, float maxPercentage, const sf::Vector2u &windowSize, const std::string &playerName)
{
    sf::Vector2u pos =
        sf::Vector2u{static_cast<unsigned int>(windowSize.x * 0.01f), static_cast<unsigned int>(windowSize.y * 0.05f)};
    sf::Vector2u size =
        sf::Vector2u{static_cast<unsigned int>(windowSize.x * 0.25f), static_cast<unsigned int>(windowSize.y * 0.02f)};

    ImVec2 rect1Pos = ImVec2(pos.x, pos.y);
    ImVec2 rect1Size = ImVec2(size.x, size.y);
    ImVec2 rect2Pos = ImVec2(pos.x, pos.y);
    ImVec2 rect2Size = ImVec2(size.x * (percentageBar / maxPercentage), size.y);

    ImGui::GetBackgroundDrawList()->AddRectFilled(
        rect1Pos, ImVec2(rect1Pos.x + rect1Size.x, rect1Pos.y + rect1Size.y), IM_COL32(153, 255, 153, 255), 10.f
    );
    ImGui::GetBackgroundDrawList()->AddRectFilled(
        rect2Pos, ImVec2(rect2Pos.x + rect2Size.x, rect2Pos.y + rect2Size.y), IM_COL32(0, 153, 0, 255), 10.f
    );
    ImGui::GetBackgroundDrawList()->AddText(
        ImVec2(rect2Pos.x, rect2Pos.y - 20), IM_COL32(255, 255, 255, 255), playerName.c_str()
    );
}

void ecs::systems::drawPlayerHealthBar(Registry &reg, const sf::Vector2u &windowSize)
{
    auto &player = reg.getComponents<ecs::component::Player>();
    auto &healths = reg.getComponents<ecs::component::Health>();
    auto &selfPlayer = reg.getComponents<ecs::component::SelfPlayer>();

    Zipper<ecs::component::Player, ecs::component::Health, ecs::component::SelfPlayer> zip(player, healths, selfPlayer);

    for (auto [player, health, _] : zip) {
        drawBar(health.currHp, health.maxHp, windowSize, player.name);
    }
}
