/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** main
*/

#include "rtype_client.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <thread>

#include "GameProtocol.hpp"
#include "game_manager.hpp"
#include "my_log.hpp"

// ! This function is call when you click on a room in the lobby array.
static void renderInsideRoom(const std::string &name, rtc::room_manager &room_manager)
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
    for (const auto &current : room_manager.get_current_room_player()) {
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
            room_manager.ask_to_leave_room();
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
        room_manager.ask_to_be_ready();
    }

    ImGui::End();
}

// ! This function is call when you are on the lobby page.
static void renderLobbyWindow(rtc::room_manager &room_manager)
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
    for (const auto &[room_name, room_data] : room_manager.get_rooms()) {
        if (!room_data.joinable) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
        }
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button(room_name.c_str()) && room_data.joinable) {
            // ! send join room
            room_manager.ask_to_join_room(room_name);
        }
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%zu / 4", room_data.player.size());
        ImGui::TableSetColumnIndex(2);
        if (ImGui::Button((std::string("Delete##") + room_name).c_str()) && room_data.joinable) {
            // !send delete
            room_manager.ask_to_delete_room(room_name);
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
        room_manager.ask_to_create_room("Room " + std::to_string(ecs::generate_shared_entity_id()));
    }

    ImGui::End();
}

void rtc::runGui(const std::shared_ptr<sf::RenderWindow> &window, rtc::room_manager &room_manager, bool &in_lobby)
{
    ImGui::SFML::Init(*window);
    sf::Clock dt;

    while (window->isOpen() && in_lobby) {
        sf::Event event;
        while (window->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }
        ImGui::SFML::Update(*window, dt.restart());
        window->clear();

        if (room_manager.get_current_room().empty()) {
            renderLobbyWindow(room_manager);
        } else {
            renderInsideRoom(room_manager.get_current_room(), room_manager);
        }

        ImGui::SFML::Render(*window);
        window->display();
    }
}

void rtc::run(
    ecs::registry &reg,
    const std::shared_ptr<sf::RenderWindow> &window,
    float &dt,
    client::UDPClient &udpClient,
    ecs::input_manager &input
)
{
    sf::Clock clock;

    while (window->isOpen()) {
        dt = clock.restart().asSeconds();

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            input.update(event);
        }
        reg.run_systems();
    }
}

int main(int argc, const char *argv[])
{
    ArgParser argParser;
    argParser.addArgument("ip", "i", ArgParser::ArgType::STRING, true, "Server IP address");
    argParser.addArgument("port", "p", ArgParser::ArgType::INT, true, "Server port");
    argParser.addArgument("player_name", "pn", ArgParser::ArgType::STRING, true, "Player name");
    argParser.addArgument("help", "h", ArgParser::ArgType::BOOL, false, "Print this help message");

    if (!argParser.parse(argc, argv)) {
        argParser.printHelp();
        return 84;
    }
    if (argParser.getValue<bool>("help")) {
        argParser.printHelp();
        return 0;
    }

    auto ip = argParser.getValue<std::string>("ip");
    auto port = argParser.getValue<int>("port");
    auto player_name = argParser.getValue<std::string>("player_name");

    rtc::game_manager game(ip, port, player_name);

    game.run_game();
    return 0;
}
