/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** run_gui
*/

#include <SFML/System/Vector2.hpp>
#include "RTypeClient.hpp"
#include "imgui.h"
#include "core/shared_entity.hpp"
#include "imgui-SFML.h"

static void renderInsideRoom(const std::string &name, rtc::RoomManager &roomManager, const sf::Vector2u &windowSize)
{
    // ! Window
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(
        name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
    );

    // ! Table
    ImGui::BeginTable("playersTable", 2);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Status");
    ImGui::TableHeadersRow();
    ImU32 selfRowColor = ImGui::GetColorU32(ImVec4(0.5, 1, 0.5, 0.25));

    // ! Action with table
    for (const auto &[id, player] : roomManager.getCurrentRoomPlayer()) {
        ImGui::TableNextRow();
        if (id == roomManager.getSelfId()) {
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, selfRowColor);
        }
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", player.name.c_str());
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", player.ready ? "ready" : "not ready");
    }
    ImGui::EndTable();

    // ! Back to lobby button
    {
        ImVec2 buttonSize(200, 50);
        ImVec2 windowContentRegionMax = ImGui::GetWindowContentRegionMax();
        ImVec2 buttonPos =
            ImVec2(windowContentRegionMax.x - buttonSize.x - 20, windowContentRegionMax.y - buttonSize.y - 20);
        ImGui::SetCursorPos(buttonPos);
        if (ImGui::Button("Back to lobby", buttonSize)) {
            // ! send leave
            roomManager.askToLeaveRoom();
        }
    }

    // ! ready button
    ImVec2 buttonSize(200, 50);
    ImVec2 windowContentRegionMax = ImGui::GetWindowContentRegionMax();
    ImVec2 buttonPos = ImVec2(20, windowContentRegionMax.y - buttonSize.y - 20);
    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("Ready", buttonSize)) {
        // ! send ready
        roomManager.askToBeReady();
    }

    ImGui::End();
}

static void renderLobbyWindow(rtc::RoomManager &roomManager, const sf::Vector2u &windowSize)
{
    // ! Window
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Lobby", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

    // ! Table
    ImGui::BeginTable("table", 3);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Number of players");
    ImGui::TableSetupColumn("Actions");
    ImGui::TableHeadersRow();
    ImU32 inGameRowColor = ImGui::GetColorU32(ImVec4(1, 0, 0, 0.25));

    // ! Action with table
    for (const auto &[room_name, room_data] : roomManager.getRooms()) {
        ImGui::TableNextRow();
        if (!room_data.joinable) {
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, inGameRowColor);
        }
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button(room_name.c_str()) && room_data.joinable) {
            // ! send join room
            roomManager.askToJoinRoom(room_name);
        }
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%zu / 4", room_data.player.size());
        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button((std::string("Delete##") + room_name).c_str()) && room_data.joinable &&
            room_data.player.empty()) {
            // !send delete
            roomManager.askToDeleteRoom(room_name);
        }
    }
    ImGui::EndTable();
    // ! Create room
    ImVec2 buttonSize(200, 50);
    ImVec2 windowContentRegionMax = ImGui::GetWindowContentRegionMax();
    ImVec2 buttonPos =
        ImVec2(windowContentRegionMax.x - buttonSize.x - 20, windowContentRegionMax.y - buttonSize.y - 20);
    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("Create", buttonSize)) {
        // ! send create room
        roomManager.askToCreateRoom("Room " + std::to_string(ecs::generateSharedEntityId()));
    }

    ImGui::End();
}

void rtc::runGui(const std::shared_ptr<sf::RenderWindow> &window, rtc::RoomManager &roomManager, bool &inLobby)
{
    if (!ImGui::SFML::Init(*window)) {
        throw std::runtime_error("IMGUI Window init failed");
    }
    sf::Clock dt;

    while (window->isOpen() && inLobby) {
        sf::Event event;
        while (window->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(*window, event);
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }
        ImGui::SFML::Update(*window, dt.restart());
        window->clear();

        if (roomManager.getCurrentRoom().empty()) {
            renderLobbyWindow(roomManager, window->getSize());
        } else {
            renderInsideRoom(roomManager.getCurrentRoom(), roomManager, window->getSize());
        }

        ImGui::SFML::Render(*window);
        window->display();
    }
}
