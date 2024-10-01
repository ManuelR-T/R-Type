/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** run_gui
*/

#include "RTypeClient.hpp"
#include "imgui.h"
#include "core/shared_entity.hpp"
#include "imgui-SFML.h"

// ! This function is call when you click on a room in the lobby array.
static void renderInsideRoom(const std::string &name, rtc::RoomManager &roomManager)
{
    // ! Window
    ImVec2 windowSize(850, 600);
    ImVec2 windowPos(100, 50);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::Begin(
        name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
    );

    // ! Table
    ImGui::BeginTable("playersTable", 2);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Status");
    ImGui::TableHeadersRow();

    // ! Action with table
    for (const auto &current : roomManager.getCurrentRoomPlayer()) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", current.first.c_str());
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", std::to_string(current.second).c_str());
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
    ImVec2 buttonPos =
        ImVec2(windowContentRegionMax.x - buttonSize.x - 620, windowContentRegionMax.y - buttonSize.y - 20);
    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("Ready", buttonSize)) {
        // ! send ready
        roomManager.askToBeReady();
    }

    ImGui::End();
}

// ! This function is call when you are on the lobby page.
static void renderLobbyWindow(rtc::RoomManager &roomManager)
{
    // ! Window
    ImVec2 windowSize(850, 600);
    ImVec2 windowPos(100, 50);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
    ImGui::Begin("Lobby", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

    // ! Table
    ImGui::BeginTable("table", 3);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Number of players");
    ImGui::TableSetupColumn("Actions");
    ImGui::TableHeadersRow();

    // ! Action with table
    for (const auto &[room_name, room_data] : roomManager.getRooms()) {
        if (!room_data.joinable) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
        }
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button(room_name.c_str()) && room_data.joinable) {
            // ! send join room
            roomManager.askToJoinRoom(room_name);
        }
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%zu / 4", room_data.player.size());
        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button((std::string("Delete##") + room_name).c_str()) && room_data.joinable) {
            // !send delete
            roomManager.askToDeleteRoom(room_name);
        }
        if (!room_data.joinable) {
            ImGui::PopStyleColor();
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
            renderLobbyWindow(roomManager);
        } else {
            renderInsideRoom(roomManager.getCurrentRoom(), roomManager);
        }

        ImGui::SFML::Render(*window);
        window->display();
    }
}
